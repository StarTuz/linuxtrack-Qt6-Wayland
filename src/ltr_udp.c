/*
 * ltr_udp.c
 *
 * Bridge to send Linuxtrack pose data via UDP in the format supported by
 * X4: Foundations and other OpenTrack-compatible games.
 *
 * Protocol: 6 doubles (little-endian)
 * [X, Y, Z, Yaw, Pitch, Roll]
 * X,Y,Z in cm.
 * Yaw, Pitch, Roll in degrees.
 *
 * Connects to localhost:4242 by default.
 */

#include <arpa/inet.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"
#include "linuxtrack.h"

// Default Opentrack UDP port
#define DEFAULT_PORT 4242
#define DEFAULT_HOST "127.0.0.1"

static volatile bool keep_running = true;
static int sock_fd = -1;
static struct sockaddr_in dest_addr;

void signal_handler(int dummy) {
  (void)dummy;
  keep_running = false;
}

// OpenTrack UDP Packet Structure: 6 doubles
// Order: X, Y, Z, Yaw, Pitch, Roll
typedef struct __attribute__((packed)) {
  double x;     // cm
  double y;     // cm
  double z;     // cm
  double yaw;   // degrees
  double pitch; // degrees
  double roll;  // degrees
} opentrack_udp_t;

int main(int argc, char *argv[]) {
  linuxtrack_pose_t pose;
  // Blobs buffer required by API even if we don't use it
  float blobs[30];
  int blobs_read;
  const char *target_ip = DEFAULT_HOST;
  int target_port = DEFAULT_PORT;

  // Parse arguments
  if (argc >= 2) {
    target_port = atoi(argv[1]);
  }
  if (argc >= 3) {
    target_ip = argv[2];
  }

  // Setup signal handling
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  // Initialize UDP socket
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0) {
    perror("Failed to create socket");
    return 1;
  }

  memset(&dest_addr, 0, sizeof(dest_addr));
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(target_port);
  if (inet_pton(AF_INET, target_ip, &dest_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    return 1;
  }

  printf("ltr_udp: Sending generic OpenTrack UDP data to %s:%d\n", target_ip,
         target_port);
  printf("Press Ctrl+C to stop.\n");

  // Initialize Linuxtrack
  linuxtrack_state_type state = linuxtrack_init(NULL);
  if (state < LINUXTRACK_OK) {
    fprintf(stderr, "Failed to initialize linuxtrack: %s\n",
            linuxtrack_explain(state));
    return 1;
  }

  // Wait for tracker to start
  int timeout = 0;
  while (timeout < 50 && keep_running) {
    state = linuxtrack_get_tracking_state();
    if (state == RUNNING || state == PAUSED) {
      break;
    }
    usleep(200000); // 200ms
    timeout++;
  }

  if (state != RUNNING && state != PAUSED) {
    fprintf(stderr, "Tracker failed to start. Is ltr_server1 running?\n");
    linuxtrack_shutdown();
    return 1;
  }

  // Main Loop
  opentrack_udp_t packet;

  while (keep_running) {
    // Wait for frame (blocking wait to reduce CPU usage)
    linuxtrack_wait(100);

    if (linuxtrack_get_pose_full(&pose, blobs, 10, &blobs_read) > 0) {
      // Linuxtrack units:
      // Translation: Millimeters (typically)
      // Rotation: Degrees

      // X4 / OpenTrack UDP expectation:
      // Translation: Centimeters (?) - Standard is usually cm for flight sims.
      // Rotation: Degrees.

      // We convert mm -> cm by dividing by 10.0
      packet.x = (double)pose.tx / 10.0;
      packet.y = (double)pose.ty / 10.0;
      packet.z = (double)pose.tz / 10.0;

      // Rotations are already in degrees
      packet.yaw = (double)pose.yaw;
      packet.pitch = (double)pose.pitch;
      packet.roll = (double)pose.roll;

      // Send packet
      sendto(sock_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr,
             sizeof(dest_addr));
    }
  }

  printf("\nShutting down...\n");
  linuxtrack_shutdown();
  close(sock_fd);
  return 0;
}
