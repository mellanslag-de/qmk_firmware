#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "action.h"
#include "action_layer.h"

// Delays of the quickshift machine (ms); override in config.h if needed.

// How long a key must be held before it counts as meant to be shifted
#ifndef QUICKSHIFT_HOLD_TIMEOUT
#    define QUICKSHIFT_HOLD_TIMEOUT 150
#endif

// Minimum gap between the correcting backspace and the shifted replacement
#ifndef QUICKSHIFT_CORRECTION_DELAY
#    define QUICKSHIFT_CORRECTION_DELAY 5
#endif

// Returns true when quickshift consumed the key; the caller then stops processing it.
bool quickshift__process_record_user(uint16_t keycode, keyrecord_t *record);
void quickshift__matrix_scan_user(void);
void quickshift__layer_set_state_user(layer_state_t state);
