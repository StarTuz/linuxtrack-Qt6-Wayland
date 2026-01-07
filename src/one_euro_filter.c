/*
 * One Euro Filter Implementation
 * 
 * Based on the algorithm described in:
 * "1€ Filter: A Simple Speed-based Low-pass Filter for Noisy Input in Interactive Systems"
 * Géry Casiez, Nicolas Roussel, Daniel Vogel
 * CHI 2012
 * https://cristal.univ-lille.fr/~casiez/1euro/
 * 
 * This implementation is original code written from the published algorithm.
 * It is provided under the same license as Linuxtrack (MIT).
 */

#include "one_euro_filter.h"
#include <math.h>
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Compute the smoothing factor (alpha) for a simple low-pass filter.
 * 
 * @param dt     Time delta (seconds)
 * @param cutoff Cutoff frequency (Hz)
 * @return       Smoothing factor alpha (0-1)
 */
static float smoothing_factor(float dt, float cutoff) {
    float tau = 1.0f / (2.0f * (float)M_PI * cutoff);
    return 1.0f / (1.0f + tau / dt);
}

/**
 * Simple exponential low-pass filter.
 * 
 * @param alpha Smoothing factor (0-1)
 * @param x     Current sample
 * @param prev  Previous filtered value
 * @return      New filtered value
 */
static float low_pass_filter(float alpha, float x, float prev) {
    return alpha * x + (1.0f - alpha) * prev;
}

void one_euro_init(one_euro_filter_t *f, float min_cutoff, float beta, float d_cutoff) {
    if (f == NULL) return;
    
    f->min_cutoff = min_cutoff;
    f->beta = beta;
    f->d_cutoff = d_cutoff;
    f->prev_x = 0.0f;
    f->prev_dx = 0.0f;
    f->initialized = false;
}

void one_euro_reset(one_euro_filter_t *f) {
    if (f == NULL) return;
    
    f->prev_x = 0.0f;
    f->prev_dx = 0.0f;
    f->initialized = false;
}

void one_euro_set_params(one_euro_filter_t *f, float min_cutoff, float beta) {
    if (f == NULL) return;
    
    f->min_cutoff = min_cutoff;
    f->beta = beta;
}

float one_euro_filter(one_euro_filter_t *f, float x, float dt) {
    if (f == NULL) return x;
    
    // Clamp dt to reasonable bounds to avoid numerical issues
    if (dt <= 0.0f) dt = 1.0f / 120.0f;  // Default to 120 fps
    if (dt > 1.0f) dt = 1.0f;            // Cap at 1 second
    
    // First sample - just store and return
    if (!f->initialized) {
        f->prev_x = x;
        f->prev_dx = 0.0f;
        f->initialized = true;
        return x;
    }
    
    // Compute derivative (rate of change)
    float dx = (x - f->prev_x) / dt;
    
    // Filter the derivative to reduce noise in the derivative estimate
    float alpha_d = smoothing_factor(dt, f->d_cutoff);
    float dx_filtered = low_pass_filter(alpha_d, dx, f->prev_dx);
    f->prev_dx = dx_filtered;
    
    // Compute adaptive cutoff frequency based on speed
    // Higher speed = higher cutoff = less smoothing = more responsive
    float cutoff = f->min_cutoff + f->beta * fabsf(dx_filtered);
    
    // Apply the low-pass filter with adaptive cutoff
    float alpha = smoothing_factor(dt, cutoff);
    float x_filtered = low_pass_filter(alpha, x, f->prev_x);
    f->prev_x = x_filtered;
    
    return x_filtered;
}
