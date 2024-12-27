#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "keycodes.h"
#include "layers.h"
#include "keymap_german.h"
#include "quantum_keycodes.h"

bool is_quickshift_active = true;
bool is_quickshift_active_at_current_layer = true;
int quickshift_timeout = 150;
bool is_quickshift_timer_active = false;
uint16_t quickshift_timer = 0;
uint16_t quickshift_keycode_of_timer = 0;

uint16_t quickshift_active_keycodes[] = {
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
    DE_SS,
};

uint16_t quickshift_special_keycode_mappings[][2] = {
    {DE_SS,                RALT(S(DE_SS))},
};

int quickshift_active_layers[] = {
    LETTERS,
    QWERTZ,
};
