#pragma once
#include "quickshift.h"

//
// Created by ddeut on 06.07.2021.
//
bool is_quickshift_active_for_keycode(uint16_t keycode) {
    for (int i = 0; i < sizeof(quickshift_active_keycodes) / sizeof(quickshift_active_keycodes[0]); i++) {
        if (quickshift_active_keycodes[i] == keycode) {
            return true;
        }
    }

    return false;
}

uint16_t get_shifted_keycode(uint16_t keycode) {
    for (int i = 0; i < sizeof(quickshift_special_keycode_mappings) / sizeof(quickshift_special_keycode_mappings[0]); i++) {
        if (quickshift_special_keycode_mappings[i][0] == keycode) {
            return quickshift_special_keycode_mappings[i][1];
        }
    }

    return S(keycode);
}
bool is_any_modifier_currently_active(void) {
    return
        get_mods() & MOD_MASK_CTRL
        || get_mods() & MOD_MASK_SHIFT
        || get_mods() & MOD_MASK_ALT
        || get_mods() & MOD_MASK_GUI
        || get_oneshot_mods() & MOD_MASK_CTRL
        || get_oneshot_mods() & MOD_MASK_SHIFT
        || get_oneshot_mods() & MOD_MASK_ALT
        || get_oneshot_mods() & MOD_MASK_GUI;
}
bool is_quickshift_currently_active(void) {
    return
        is_quickshift_active
        && is_quickshift_active_at_current_layer
        && !is_any_modifier_currently_active();
}

void disable_timer_if_modifier_was_pressed(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_LCTL:  // fall through
            case KC_RCTL:  // fall through
            case KC_LSFT: // fall through
            case KC_RSFT: // fall through
            case KC_LALT:   // fall through
            case KC_RALT:   // fall through
            case KC_LGUI:   // fall through
            case KC_RGUI:
                is_quickshift_timer_active = false;
        }
    }
}

bool quickshift__process_record_user(uint16_t keycode, keyrecord_t *record) {
    disable_timer_if_modifier_was_pressed(keycode, record);

    if (
        is_quickshift_currently_active()
        && is_quickshift_active_for_keycode(keycode)
        && !is_caps_word_on()
    ) {
        if (record->event.pressed) {
            register_code(keycode);
            unregister_code(keycode);

            is_quickshift_timer_active  = true;
            quickshift_timer            = record->event.time;
            quickshift_keycode_of_timer = keycode;
        } else {
            is_quickshift_timer_active = false;
        }

        return true;
    }

    return false;
}

void quickshift__matrix_scan_user(void) {
    if (
        is_quickshift_currently_active()
        && is_quickshift_timer_active
        && timer_elapsed(quickshift_timer) > quickshift_timeout
    ) {
        uint16_t shifted_keycode = get_shifted_keycode(quickshift_keycode_of_timer);

        register_code(KC_BSPC);
        unregister_code(KC_BSPC);

        register_code16(shifted_keycode);
        unregister_code16(shifted_keycode);

        is_quickshift_timer_active = false;
    }
}

void quickshift__layer_set_state_user(layer_state_t state) {
    int current_layer = get_highest_layer(state);

    for (int i = 0; i < sizeof(quickshift_active_layers) / sizeof(quickshift_active_layers[0]); i++) {
        if (current_layer == quickshift_active_layers[i]) {
            is_quickshift_active_at_current_layer = true;
            return;
        }
    }
    is_quickshift_active_at_current_layer = false;
}

