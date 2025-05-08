#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "keycodes.h"
#include "layers.h"
#include "keymap_german.h"
#include "quantum_keycodes.h"

enum QUICKSHIFT_STATES {
    INACTIVE__AWAITING_KEYPRESS,
    KEY_PRESSED__AWAITING_RELEASE,
    TRIGGERED_BACKSPACE__CHAR_TO_BE_PRESSED_AFTER_DELAY,
};

bool is_quickshift_active = true;
bool is_quickshift_active_at_current_layer = true;

enum QUICKSHIFT_STATES quickshift_timer_state = INACTIVE__AWAITING_KEYPRESS;
int quickshift_timer = 0;
uint16_t quickshift_timer_keycode = 0;

// Timer when quickshift initially triggers
int quickshift_trigger_timer_timeout = 150;

// Timer when quickshift triggers the keypress, once it triggered and immediately sent backspace
int quickshift_char_timer_timeout = 5;

uint16_t quickshift_active_keycodes_basic[] = {
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
};

uint16_t quickshift_active_keycodes_special[] = {
    DE_SS,
    KC_2,
    KC_6
};

uint16_t quickshift_special_keycode_mappings[][2] = {
    {DE_SS, RALT(LSFT(DE_SS))},
    {KC_2,  LSFT(KC_3)},
    {KC_6,  RALT(KC_E)},
};

int quickshift_active_layers[] = {
    LETTERS,
    QWERTZ,
};

