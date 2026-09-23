#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "action.h"

// Paints the current layer's ledmap colours; call from rgb_matrix_indicators_user.
bool rgb__rgb_matrix_indicators_user(void);
// Handles the M_RGB_* colour macros. Returns false when the key was handled.
bool rgb__process_record_user(uint16_t keycode, keyrecord_t *record);
