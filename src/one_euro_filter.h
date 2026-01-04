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

#ifndef ONE_EURO_FILTER_H
#define ONE_EURO_FILTER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * One Euro Filter state structure.
 * Each axis needs its own filter instance.
 */
typedef struct {
    float min_cutoff;    // Minimum cutoff frequency (Hz) - controls jitter (higher = less smoothing)
    float beta;          // Speed coefficient - controls lag (higher = more responsive to fast movements)
    float d_cutoff;      // Derivative cutoff frequency (Hz) - usually 1.0
    
    // Internal state
    float prev_x;        // Previous filtered value
    float prev_dx;       // Previous derivative (filtered)
    bool initialized;    // Has the filter been initialized with first sample?
} one_euro_filter_t;

/**
 * Initialize a One Euro filter with the given parameters.
 * 
 * @param f         Pointer to filter state
 * @param min_cutoff Minimum cutoff frequency (Hz). Higher = less smoothing. Typical: 0.5-5.0
 * @param beta      Speed coefficient. Higher = more responsive. Typical: 0.0-0.1
 * @param d_cutoff  Derivative cutoff (Hz). Usually 1.0.
 */
void one_euro_init(one_euro_filter_t *f, float min_cutoff, float beta, float d_cutoff);

/**
 * Reset filter state (for when tracking is recentered, etc.)
 */
void one_euro_reset(one_euro_filter_t *f);

/**
 * Apply the One Euro filter to a new sample.
 * 
 * @param f   Pointer to filter state
 * @param x   New sample value
 * @param dt  Time delta since last sample (seconds)
 * @return    Filtered value
 */
float one_euro_filter(one_euro_filter_t *f, float x, float dt);

/**
 * Update filter parameters without resetting state.
 */
void one_euro_set_params(one_euro_filter_t *f, float min_cutoff, float beta);

#ifdef __cplusplus
}
#endif

#endif /* ONE_EURO_FILTER_H */
