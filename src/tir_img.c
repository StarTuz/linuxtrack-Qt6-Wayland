#include "list.h"
#include <stdio.h>
#include <time.h>
#include "usb_ifc.h"
#include "tir_hw.h"
#include "tir_img.h"
#include "utils.h"
#include "image_process.h"
#include "sn4_com.h"
#include <assert.h>

static unsigned int pkt_no = 0;
static image_t *p_img = NULL;
static unsigned int current_line = 0;
static dev_found device = NOT_TIR;

// Simple consecutive error tracking for USB desync detection
// Trigger recovery on 30 consecutive errors (burst detection)
// At 120Hz, this is 250ms of continuous bad data
#define ERROR_THRESHOLD 30

// Escalation: after this many failed camera resets, do USB reset
#define FAILED_RESET_ESCALATION_THRESHOLD 60

// Time-based stall detection: if no valid frame for this many seconds, escalate to USB reset
#define STALL_TIMEOUT_SECONDS 60

static int consecutive_errors = 0;
static int failed_camera_resets = 0;  // Camera resets without successful frame
static bool awaiting_post_reset_frame = false;  // Track if we're waiting for first good frame after reset

// Time-based stall detection
static time_t last_good_frame_time = 0;
static bool stall_detection_active = false;

// Flag indicating an external USB reset occurred - tracking loop should reset its state
// This is set by ltr_int_reset_usb_tir() when called from IPC, to notify the tracking loop
static volatile bool external_usb_reset_occurred = false;

// Called by ltr_int_reset_usb_tir() to notify tracking loop that a reset happened
void ltr_int_signal_usb_reset_occurred(void)
{
  external_usb_reset_occurred = true;
}

// Check and clear the external reset flag - tracking loop resets its state when this is true
static bool check_and_clear_external_reset(void)
{
  if (external_usb_reset_occurred) {
    external_usb_reset_occurred = false;
    return true;
  }
  return false;
}

static void record_packet_error(void) {
    consecutive_errors++;
}

static int count_recent_errors(void) {
    return consecutive_errors;
}

static void reset_error_tracking(void) {
    consecutive_errors = 0;
}

static void reset_on_good_packet(void) {
    // Reset counter when we get a valid packet
    // This distinguishes real recovery from garbage that happens to parse
    consecutive_errors = 0;

    // If we were awaiting a good frame after reset, we got one - reset succeeded!
    if (awaiting_post_reset_frame) {
        awaiting_post_reset_frame = false;
        failed_camera_resets = 0;  // Camera reset worked, clear escalation counter
    }
}

static bool process_stripe_tir2(unsigned char p_stripe[])
{
  stripe_t stripe;
    stripe.vline = p_stripe[0];
    stripe.hstart = p_stripe[1];
    stripe.hstop = p_stripe[2];
    stripe.sum = stripe.hstop - stripe.hstart + 1;
    stripe.sum_x = (unsigned int)(stripe.sum * (stripe.sum - 1) / 2.0);
    stripe.points = stripe.sum;
    if(!ltr_int_add_stripe(&stripe, p_img)){
      ltr_int_log_message("Couldn't add stripe!\n");
    }
  return true;
}
static bool process_stripe_sn4(unsigned char p_stripe[])
{
  stripe_t stripe;
  unsigned char rest;

    stripe.vline = p_stripe[0];
    stripe.hstart = p_stripe[1];
    stripe.hstop = p_stripe[2];
    rest = p_stripe[3];
    if(rest & 0x01)
      stripe.hstop |= 0x100;
    if(rest & 0x02)
      stripe.hstart |= 0x100;
    if(rest & 0x04)
      stripe.vline |= 0x100;
    if(rest & 0x08)
      stripe.hstop |= 0x200;
    if(rest & 0x10)
      stripe.hstart |= 0x200;
    if(rest & 0x20)
      stripe.vline |= 0x200;
    if(rest & 0x40)
      stripe.hstop |= 0x400;
    if(rest & 0x80)
      stripe.hstart |= 0x400;
    stripe.sum = stripe.hstop - stripe.hstart + 1;
    stripe.sum_x = (unsigned int)(stripe.sum * (stripe.sum - 1) / 2.0);
    stripe.points = stripe.sum;
    if(!ltr_int_add_stripe(&stripe, p_img)){
      ltr_int_log_message("Couldn't add stripe!\n");
    }
  return true;
}

static bool process_stripe_sn4gr(unsigned char p_stripe[], size_t size)
{
  stripe_t stripe;
  unsigned char rest;
  size_t i, j;
  unsigned int last_vline = 0;

  if(size < 4){
    return false;
  }
  i = 0;

  while(i < (size - 4)){
    stripe.hstart = p_stripe[i];
    stripe.vline = p_stripe[i + 1];
    rest = p_stripe[i + 2];
    if(rest & 0x01)
      stripe.hstart |= 0x100;
    if(rest & 0x02)
      stripe.hstart |= 0x200;
    if(rest & 0x04)
      stripe.vline |= 0x100;
    stripe.sum = stripe.sum_x = 0;
    if(stripe.vline < last_vline){
//      printf("%d x %d!!!!\n", stripe.vline, last_vline);
//      for(i = 0; i < size; ++i){
//        printf("%02X ", p_stripe[i]);
//      }
      break;
    }
    last_vline = stripe.vline;
    i += 3;
    j = 1;
    while((p_stripe[i] != 0)&&(i < size)){
      stripe.sum += p_stripe[i];
      stripe.sum_x += (j * p_stripe[i]);
      ++i;
      ++j;
    }
    stripe.points = j - 1;
    stripe.hstop = stripe.hstart + stripe.points - 1;
    if(p_stripe[i] == 0){
      ++i;
      //printf("%ld x (%d %d) y %d (%d points)\n", i, stripe.hstart,
      //  stripe.hstop, stripe.vline, stripe.points);
      if(!ltr_int_add_stripe(&stripe, p_img)){
        ltr_int_log_message("Couldn't add stripe!\n");
      }
    }else{
      //reached the end of packet...
      //printf(">> %02X <<(%ld %ld)\n", p_stripe[i], i, size);
      break;
    }

  }
  return true;
}



static bool process_stripe_tir4(unsigned char p_stripe[])
{
  stripe_t stripe;
  unsigned char rest;

    stripe.vline = p_stripe[0];
    stripe.hstart = p_stripe[1];
    stripe.hstop = p_stripe[2];
    rest = p_stripe[3];
    if(rest & 0x20)
      stripe.vline |= 0x100;
    if(rest & 0x80)
      stripe.hstart |= 0x100;
    if(rest & 0x40)
      stripe.hstop |= 0x100;
    if(rest & 0x10)
      stripe.hstart |= 0x200;
    if(rest & 0x08)
      stripe.hstop |= 0x200;
//    assert(stripe.hstart >= 81);
//    assert(stripe.hstop >= 81);
//    assert(stripe.vline >= 12);
//    stripe.hstart -= 81;
//    stripe.vline -= 12;
//    stripe.hstop -= 81;
    stripe.sum = stripe.hstop - stripe.hstart + 1;
    stripe.sum_x = (unsigned int)(stripe.sum * (stripe.sum - 1) / 2.0);
    stripe.points = stripe.sum;
    if(!ltr_int_add_stripe(&stripe, p_img)){
      ltr_int_log_message("Couldn't add stripe!\n");
    }
//    ltr_int_log_message("Stripe: %d\t\t%d\t\t%d %d %d %d\n", stripe.hstart, stripe.hstop, stripe.vline, stripe.sum,
//           stripe.sum_x, stripe.points);
  return true;
}

static bool is_next_frame_tir(unsigned char p_stripe[])
{
  unsigned int vline = p_stripe[0];
  bool res;
  if(p_stripe[3] & 0x20)
    vline |= 0x100;
  if(vline < current_line){
    res = true;
  }else{
    res = false;
  }
  current_line = vline;
  return res;
}

static bool is_next_frame_tir2(unsigned char p_stripe[])
{
  unsigned int vline = p_stripe[0];
  bool res;
  if(vline < current_line){
    res = true;
  }else{
    res = false;
  }
  current_line = vline;
  return res;
}

static bool process_stripe_tir5(unsigned char payload[])
{
  stripe_t stripe;
    stripe.hstart = (((unsigned int)payload[0]) << 2) |
                     (((unsigned int)payload[1]) >> 6);
    stripe.vline = ((((unsigned int)payload[1]) & 0x3F) << 3) |
                    ((((unsigned int)payload[2]) & 0xE0) >> 5);
    stripe.points = (((((unsigned int)payload[2]) & 0x1F) << 5) |
                    (((unsigned int)payload[3]) >> 3));
    stripe.hstop =  stripe.points + stripe.hstart - 1;
    stripe.sum_x = (((unsigned int)payload[3]) & 7) << 17 |
                    (((unsigned int)payload[4]) << 9) |
		    ((unsigned int)payload[5]) << 1 |
		    ((unsigned int)payload[6]) >>7;
    stripe.sum = (((unsigned int)payload[6]) & 0x7F) << 8 |
                   ((unsigned int)payload[7]);
    if(!ltr_int_add_stripe(&stripe, p_img)){
      ltr_int_log_message("Couldn't add stripe!\n");
    }
  return true;
}

static bool check_paket_header_tir5(unsigned char data[])
{
  if((data[0] ^ data[1] ^ data[2] ^ data[3]) != 0xAA){
    record_packet_error();
    int recent = count_recent_errors();
    if(recent <= 5){
      ltr_int_log_message("Bad packet header! [%d consecutive]\n", recent);
    }
    return false;
  }else{
    return true;
  }
}

static bool check_paket_header_sn4(unsigned char data[])
{
  unsigned char csum = 0;
  int i;
  for(i = 0; i < 8; ++i){
    csum ^= data[i];
  }
  if(csum != 0xAA){
    record_packet_error();
    int recent = count_recent_errors();
    if(recent <= 5){
      ltr_int_log_message("Bad packet header! [%d consecutive]\n", recent);
    }
    return false;
  }else{
    return true;
  }
}


static bool process_packet_tir5(unsigned char data[], size_t *ptr, unsigned int pktsize, int limit)
{
  bool have_frame = false;
  unsigned int ps = 0;
  unsigned char type = data[*ptr + 2];

  if((type == 0) || (type == 5)){
    if(limit < 4){
      record_packet_error();
      int recent = count_recent_errors();
      if(recent <= 5){
        ltr_int_log_message("Bad packet header (limit<4)! [%d consecutive]\n", recent);
      }
      *ptr = limit;
      return false;
    }
    if(!check_paket_header_tir5(&(data[*ptr]))){
      // Error already counted and logged in check_paket_header_tir5
      *ptr = limit;
      return false;
    }
    ps = data[limit - 4];
    ps = (ps << 8) + data[limit - 3];
    ps = (ps << 8) + data[limit - 2];
    ps = (ps << 8) + data[limit - 1];
    if(ps != (pktsize - 8)){
      record_packet_error();
      int recent = count_recent_errors();
      if(recent <= 5){
        ltr_int_log_message("Bad packet size! %d x %d [%d consecutive]\n", ps, pktsize - 8, recent);
      }
      *ptr = limit;
      return false;
    }
  }

  switch(type){
    case 0:
    case 5:
      pkt_no = data[*ptr];
      *ptr += 4;
      while(1){
	if(type == 0){
          if(ps < 4){
            break;
          }
	  process_stripe_tir4((unsigned char *)&(data[*ptr]));
	  *ptr += 4;
          ps -= 4;
	}else{
          if(ps < 8){
            break;
          }
	  process_stripe_tir5((unsigned char *)&(data[*ptr]));
	  *ptr += 8;
          ps -= 8;
	}
      }
      have_frame = true;
      break;
    case 3:
      //These are most probably B/W data from camera without any preprocessing..
      //We ignore them.
      break;
  }
  return have_frame;
}

static bool process_packet_sn4(unsigned char data[], size_t *ptr, unsigned int pktsize, int limit)
{
  bool have_frame = false;
  unsigned int ps = 0;
  static unsigned char prev_btns = 3;
  unsigned char btns = data[*ptr];
  unsigned char type = data[*ptr + 1];

  if((limit < 12) || !check_paket_header_sn4(&(data[*ptr]))){
    *ptr = limit;
    return false;
  }

  ps = data[limit - 4];
  ps = (ps << 8) + data[limit - 3];
  ps = (ps << 8) + data[limit - 2];
  ps = (ps << 8) + data[limit - 1];
  if(ps != (pktsize - 4)){
    *ptr = limit;
    record_packet_error();
    int recent = count_recent_errors();
    if(recent <= 5){
      ltr_int_log_message("Bad packet size! %d x %d [%d consecutive]\n", ps, pktsize - 8, recent);
    }
    return false;
  }

  if(btns != prev_btns){
    sn4_btn_event_t ev;
    ev.btns = btns;
    prev_btns = btns;
    gettimeofday(&(ev.timestamp), NULL);
    ltr_int_send_sn4_data((void*)&ev, sizeof(ev));
//    printf("Sending %d!\n", btns);
  }

  ps -= 8; // header
  switch(type){
    case 4:
      if(ps > 8){
        ps -= 4; //skip threshold
        *ptr += (4 + 8); //header + threshold
        have_frame = process_stripe_sn4gr((unsigned char *)&(data[*ptr]), ps);
      }else{
        have_frame = false;
      }
      *ptr += limit;
      break;
    case 0:
      pkt_no = data[*ptr+4];
      pkt_no = (pkt_no << 8) + data[*ptr+5];
      pkt_no = (pkt_no << 8) + data[*ptr+6];
      pkt_no = (pkt_no << 8) + data[*ptr+7];
      //printf("%d\n", pkt_no);
      *ptr += 8;
      while(ps > 0){
        process_stripe_sn4((unsigned char *)&(data[*ptr]));
	*ptr += 4;
        ps -= 4;
      }
      *ptr += 4;
      have_frame = true;
      break;
    default:
      *ptr += limit;
      break;
  }
  return have_frame;
}

static bool process_packet_tir4(unsigned char data[], size_t *ptr, int pktsize, unsigned int limit)
{
  (void) pktsize;
  unsigned int *ui;
  bool have_frame = false;

  ui = (unsigned int *)&(data[*ptr]);
  bool go_on = true;
  do{
    if(*ui == 0){
      current_line = 0;
      have_frame = true;
      go_on = false;
      ++ui;
      (*ptr) += 4;
    }else if(is_next_frame_tir((unsigned char *)ui)){
      have_frame = true;
      go_on = false;
    }else{
//      log_message("\t%02X%02X%02X%02X\n", data[*ptr], data[*ptr + 1],
//             data[*ptr + 2], data[*ptr + 3]);
//!!!!      assert((data[*ptr + 3] & 7) == 0);
      process_stripe_tir4((unsigned char *)ui);
      ++ui;
      (*ptr) += 4;
    }
    if(*ptr >= limit){
//      log_message(">>>  size %d, limit %d, ptr %d\n", pktsize, limit, *ptr);
      go_on = false;
      if(pktsize == 0x3E){
	*ptr += 2;
      }
    }
  }while(go_on);
  return have_frame;
}


static bool process_packet_tir2(unsigned char data[], size_t *ptr, int pktsize, unsigned int limit)
{
  (void) pktsize;
  uint8_t *ui;
  bool have_frame = false;

  ui = (uint8_t *)&(data[*ptr]);
  bool go_on = true;
  do{
    if((ui[0] == 0) && (ui[1] == 0) && (ui[2] == 0)){
      current_line = 0;
      have_frame = true;
      go_on = false;
      ui += 3;
      (*ptr) += 3;
    }else if(is_next_frame_tir2(ui)){
      ltr_int_log_message("Have frame!!!!!!\n");
      have_frame = true;
      go_on = false;
    }else{
//      log_message("\t%02X%02X%02X%02X\n", data[*ptr], data[*ptr + 1],
//             data[*ptr + 2], data[*ptr + 3]);
//!!!!      assert((data[*ptr + 3] & 7) == 0);
      process_stripe_tir2((unsigned char *)ui);
      ui += 3;
      (*ptr) += 3;
    }
    if(*ptr >= limit){
//      log_message(">>>  size %d, limit %d, ptr %d\n", pktsize, limit, *ptr);
      go_on = false;
      if(pktsize == 0x3E){
	*ptr += 2;
      }
    }
  }while(go_on);
  return have_frame;
}

// Reset packet parser state (call when desync detected)
void ltr_int_reset_packet_parser(void)
{
  reset_error_tracking();
  ltr_int_log_message("Resetting packet parser state due to desync\n");
}

bool process_packet(unsigned char data[], size_t *ptr, size_t size)
{
  static int type = -1;
  static unsigned int limit = -1;
  static int pktsize = 0;
  bool have_frame;

  while(1){
    if(*ptr >= size){
      return false;
    }

    size_t start_ptr = *ptr;
    have_frame = false;
    
    if(type == -1){
      type = data[(*ptr) + 1];
//      log_message("Packet type %02X\n", type);
      switch(type){
        case 0x20:
        case 0x40:
        case 0x1C:
          pktsize = data[*ptr];
          limit= (*ptr) + pktsize;
          *ptr += 2;
          break;
        case 0x10:
          if((data[(*ptr) + 2] == 0) || (data[(*ptr) + 2] == 5)){
            pktsize = (unsigned int)size;
            limit= (*ptr) + pktsize;
          }else{
            pktsize = data[*ptr];
            // Safety: if pktsize is 0 or too small, force it to at least 2 to advance past header
            if(pktsize < 2) pktsize = 2;
            limit= (*ptr) + pktsize;
          }
          break;
        case 0x00:
        case 0x04:
          pktsize = (unsigned int)size;
          limit= (*ptr) + pktsize;
          break;

        default:
          record_packet_error();
          {
            int recent = count_recent_errors();
            if(recent <= 5){
              ltr_int_log_message("ERROR!!! ('%02X %02X') [%d consecutive]\n",
                                  data[*ptr], data[*ptr + 1], recent);
            }
          }
          // Reset parser state on desync
          type = -1;
          limit = -1;
          pktsize = 0;
	  *ptr = size; //Read new packet...
          return false;
          break;
      }
    }
    
    if(limit > size){
      *ptr = size; // Discard partial packet to prevent spin
      break; 
    }
    
    switch(type){
      case 0x20:
        //Status packet
        *ptr = limit;
        type = -1;
        break;
      case 0x40:
        //DevInfo packet
        *ptr = limit;
        type = -1;
        break;
      case 0x10:
        //TIR5 packet
        have_frame = process_packet_tir5(data, ptr, pktsize, limit);
        *ptr = limit;
        type = -1;
        break;
      case 0x1C:
        //TIR4 packet
        if(device == TIR2){
          have_frame = process_packet_tir2(data, ptr, pktsize, limit);
        }else{
          have_frame = process_packet_tir4(data, ptr, pktsize, limit);
        }
	if(*ptr >= limit){
          type = -1;
	}
        //log_message("   size %d, limit %d, ptr %d\n", pktsize, limit, *ptr);
        break;
      case 0x04: //SmartNav4 greyscale data
      case 0x00: //SmartNav4 normal data
        have_frame = process_packet_sn4(data, ptr, pktsize, limit);
        *ptr = limit;
        type = -1;
        break;
      default:
        type = -1;
        break;
    }

    // Global Safety Guard: If we are stuck in the loop without advancing or getting a frame,
    // force-advance to break the spin. This prevents high-CPU system freezes.
    if(*ptr <= start_ptr && !have_frame){
      static int spin_warning_count = 0;
      if(++spin_warning_count % 100 == 1){
        ltr_int_log_message("Stuck parser detected at %zu (type %02X), force-advancing.\n", *ptr, type);
      }
      (*ptr)++;
      type = -1;
    }

    if(have_frame == true){
      // Reset error counter on successful frame
      reset_on_good_packet();
      break;
    }
  }
  return have_frame;
}




// Max USB reads before returning (prevents infinite loop)
// At ~1ms/read, 15 reads = 15ms max blocking per frame attempt
#define MAX_USB_READS_PER_FRAME 15
static int usb_no_frame_warning_count = 0;

// Track consecutive calls that return no valid frame
// If too many (stalled), force camera reset
static int consecutive_no_frame_calls = 0;
#define MAX_NO_FRAME_BEFORE_RESET 10000  // ~10 seconds at 1ms/call

int ltr_int_read_blobs_tir(struct bloblist_type *blt, int min, int max, image_t *img, tir_info *info)
{
  assert(blt != NULL);
  assert(img != NULL);
  device = info->dev_type;
  p_img = img;
  static size_t size = 0;
  static size_t ptr = 0;
  bool have_frame = false;
  int read_count = 0;
  
  while(1){
    // Check if an external USB reset happened (from hotkey/IPC)
    // If so, reset our internal state to match the fresh USB state
    if(check_and_clear_external_reset()){
      ltr_int_log_message("External USB reset detected - resetting tracking state.\n");
      // Reset all tracking state - the USB reset was already done by the IPC handler
      reset_error_tracking();
      failed_camera_resets = 0;
      awaiting_post_reset_frame = true;
      consecutive_no_frame_calls = 0;
      last_good_frame_time = time(NULL);  // Reset stall timer
      ptr = 0;
      size = 0;
      return 0;  // Return to let caller continue with fresh state
    }

    // Time-based stall detection: if no valid frame for too long, escalate to USB reset
    if(stall_detection_active && last_good_frame_time > 0){
      time_t now = time(NULL);
      time_t stall_duration = now - last_good_frame_time;
      if(stall_duration >= STALL_TIMEOUT_SECONDS){
        ltr_int_log_message("STALL DETECTED: No valid frames for %ld seconds - forcing USB reset!\n",
                            (long)stall_duration);
        ltr_int_reset_usb_tir();
        // Reset all state
        reset_error_tracking();
        failed_camera_resets = 0;
        awaiting_post_reset_frame = true;
        consecutive_no_frame_calls = 0;
        last_good_frame_time = time(NULL);  // Reset stall timer
        ptr = 0;
        size = 0;
        return 0;
      }
    }

    if(ptr >= size){
      ptr = 0;
      if(!ltr_int_receive_data(ltr_int_data_in_ep, ltr_int_packet, sizeof(ltr_int_packet), &size, 100)){
	ltr_int_log_message("Problem reading data from USB!\n");
        return -1;
      }
      // Increment read count on EVERY read attempt (including empty/timeout reads)
      read_count++;
      
      // Prevent infinite loop - return after too many reads without a complete frame
      // This catches both: 1) data but no frame, 2) timeouts returning size=0
      if(read_count >= MAX_USB_READS_PER_FRAME){
        if(++usb_no_frame_warning_count % 100 == 1){
          ltr_int_log_message("Warning: No frame after %d USB reads, size=%zu (occurrence #%d)\n",
                              read_count, size, usb_no_frame_warning_count);
        }
        // Track stall - this return path also counts as no valid frame
        consecutive_no_frame_calls++;
        if(consecutive_no_frame_calls >= MAX_NO_FRAME_BEFORE_RESET){
          static int stall_reset_count = 0;
          stall_reset_count++;
          ltr_int_log_message("Stall detected (max reads path): %d calls, forcing reset #%d\n",
                              consecutive_no_frame_calls, stall_reset_count);
          ltr_int_pause_tir();
          ltr_int_usleep(500000);
          ltr_int_resume_tir();
          ltr_int_usleep(100000);
          reset_error_tracking();
          consecutive_no_frame_calls = 0;
          size = 0;
          ptr = 0;
        }
        return 0;
      }
      
      // If read returned 0 bytes (timeout), immediately try again but still count it
      if(size == 0){
        continue;
      }
    }
    if((have_frame = process_packet(ltr_int_packet, &ptr, size)) == true){
      break;
    }

    // Check for persistent desync - only log for debugging now.
    // We no longer trigger camera resets based on packet errors, because
    // TIR5V3 hardware sends many "unrecognized" but harmless packets.
    /*
    int recent_errors = count_recent_errors();
    if(recent_errors >= ERROR_THRESHOLD){
       ltr_int_log_message("Protocol noise: %d consecutive errors/unknown packets skipped.\n", recent_errors);
       reset_error_tracking();
    }
    */

    if(ltr_int_got_new_request()){
      break;
    }
  }

  if(have_frame){
    consecutive_no_frame_calls = 0;  // Reset stall counter on valid frame
    last_good_frame_time = time(NULL);  // Record time of good frame for stall detection
    stall_detection_active = true;  // We've seen at least one frame, enable stall detection
    int res = ltr_int_stripes_to_blobs(MAX_BLOBS, blt, min, max, img);
/*
    if(pic != NULL){
      static int fc = 0;
      char name[] = "fXXXXXXX.raw";
      sprintf(name, "f%02X%04d.raw", pkt_no, fc++);
      printf("%s\n", name);
      FILE *f = fopen(name, "wb");
      if(f != NULL){
	fwrite(pic, 1, x * y, f);
	fclose(f);
      }
    }
*/
    return res;
  }else{
    // Track stalled state - no valid frames
    consecutive_no_frame_calls++;
    if(consecutive_no_frame_calls >= MAX_NO_FRAME_BEFORE_RESET){
      static int stall_reset_count = 0;
      stall_reset_count++;
      ltr_int_log_message("Stall detected: %d calls without valid frame, forcing reset #%d\n",
                          consecutive_no_frame_calls, stall_reset_count);

      // Force camera reset
      ltr_int_pause_tir();
      ltr_int_usleep(500000);  // 500ms pause for stall recovery
      ltr_int_resume_tir();
      ltr_int_usleep(100000);  // 100ms stabilize

      // Flush data
      size_t flush_size;
      int flush_count = 0;
      while(flush_count < 50){
        if(!ltr_int_receive_data(ltr_int_data_in_ep, ltr_int_packet,
                                  sizeof(ltr_int_packet), &flush_size, 20)){
          break;
        }
        if(flush_size == 0) break;
        flush_count++;
      }

      // Reset all state
      reset_error_tracking();
      consecutive_no_frame_calls = 0;
      size = 0;
      ptr = 0;
      ltr_int_log_message("Stall reset complete, flushed %d packets\n", flush_count);
    }
    return 0;
  }
}

