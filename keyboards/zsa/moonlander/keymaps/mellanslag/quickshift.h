#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "keycodes.h"
#include "layers.h"
#include "keymap_german.h"
#include "quantum_keycodes.h"

// Delays of the quickshift machine (ms); override in config.h if needed.

// How long a key must be held before it counts as meant to be shifted
#ifndef QUICKSHIFT_HOLD_TIMEOUT
#    define QUICKSHIFT_HOLD_TIMEOUT 150
#endif

// Minimum gap between the correcting backspace and the shifted replacement
#ifndef QUICKSHIFT_CORRECTION_DELAY
#    define QUICKSHIFT_CORRECTION_DELAY 5
#endif

// Runtime switches, read by the machine's guards. They are not part of the context.
bool is_quickshift_active = true;
bool is_quickshift_active_at_current_layer = true;

static const uint16_t quickshift_keycodes[] = {
    KC_A,
    KC_B,
    KC_C,
    KC_D,
    KC_E,
    KC_F,
    KC_G,
    KC_H,
    KC_I,
    KC_J,
    KC_K,
    KC_L,
    KC_M,
    KC_N,
    KC_O,
    KC_P,
    KC_Q,
    KC_R,
    KC_S,
    KC_T,
    KC_U,
    KC_V,
    KC_W,
    KC_X,
    KC_Y,
    KC_Z,
    DE_ADIA,
    DE_ODIA,
    DE_UDIA,
    KC_2,
    DE_SS,
    KC_6,
};

static const uint8_t quickshift_active_layers[] = {
    LETTERS,
    QWERTZ,
};
