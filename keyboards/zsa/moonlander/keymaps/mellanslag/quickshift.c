#pragma once
#include "quickshift.h"
#include <stdbool.h>
#include <stdint.h>
#include "action.h"
#include "action_util.h"
#include "quantum.h"
#ifdef KEY_OVERRIDE_ENABLE
#    include "process_key_override.h"
#endif
#include "caps_word.c"

//
// Created by ddeut on 06.07.2021.
//

bool array_contains(uint16_t *arr, int size, uint16_t val) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == val) {
            return true;
        }
    }
    return false;
}

bool is_quickshift_active_for_keycode(uint16_t keycode) {
    return array_contains(quickshift_keycodes, sizeof(quickshift_keycodes), keycode);
}

#ifdef KEY_OVERRIDE_ENABLE
/**
 * Mirror of QMK's internal key_override_matches_active_modifiers.
 * Handles bitwise logic for one-sided vs both-sided modifier matching.
 */
static bool quickshift_matches_modifiers(const key_override_t *override, uint8_t active_mods) {
    if ((override->negative_mod_mask & active_mods) != 0) {
        return false;
    }

    if (override->trigger_mods == 0) {
        return true;
    }

    if ((override->options & ko_option_one_mod) != 0) {
        return (override->trigger_mods & active_mods) != 0;
    } else {
        uint8_t required_one_sided = (override->trigger_mods & 0b1111) | (override->trigger_mods >> 4);
        uint8_t active_required    = override->trigger_mods & active_mods;
        uint8_t active_one_sided   = (active_required & 0b1111) | (active_required >> 4);
        return active_one_sided == required_one_sided;
    }
}

/**
 * Iterates through user-defined key overrides to find a match for the current keycode.
 */
static const key_override_t *find_active_override(uint16_t keycode, uint8_t active_mods) {
    if (key_overrides == NULL) {
        return NULL;
    }

    uint8_t current_layer = get_highest_layer(layer_state);

    for (uint8_t i = 0; key_overrides[i] != NULL; i++) {
        const key_override_t *current_override = key_overrides[i];

        if (current_override->trigger == keycode &&
            (current_override->layers & (1UL << current_layer)) &&
            (current_override->enabled == NULL || (*current_override->enabled & 1)) &&
            quickshift_matches_modifiers(current_override, active_mods)) {
            return current_override;
        }
    }
    return NULL;
}
#endif

uint16_t get_shifted_keycode(uint16_t keycode) {
#ifdef KEY_OVERRIDE_ENABLE
    const key_override_t *matched_override = find_active_override(keycode, MOD_MASK_SHIFT);
    if (matched_override && matched_override->replacement != KC_NO) {
        return matched_override->replacement;
    }
#endif

    return LSFT(keycode);
}

bool is_any_modifier_currently_active(void) {
    uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();

    return
        mods & MOD_MASK_CTRL
        || mods & MOD_MASK_SHIFT
        || mods & MOD_MASK_ALT
        || mods & MOD_MASK_GUI;
}

bool is_only_shift_modifier_currently_active(void) {
    uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
    return (mods & ~MOD_MASK_SHIFT) == 0 && (mods & MOD_MASK_SHIFT);
}

bool is_quickshift_currently_active(void) {
    return
        is_quickshift_active
        && is_quickshift_active_at_current_layer;
}

void disable_timer_if_modifier_was_pressed(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case KC_LCTL:   // fall through
            case KC_RCTL:   // fall through
            case KC_LSFT:   // fall through
            case KC_RSFT:   // fall through
            case KC_LALT:   // fall through
            case KC_RALT:   // fall through
            case KC_LGUI:   // fall through
            case KC_RGUI:
                quickshift_timer_state = INACTIVE__AWAITING_KEYPRESS;
        }
    }
}

bool quickshift__process_record_user(uint16_t keycode, keyrecord_t *record) {
    disable_timer_if_modifier_was_pressed(keycode, record);

    if (
        is_quickshift_currently_active()
        && !is_caps_word_on()

    ) {
        if (!is_any_modifier_currently_active() && is_quickshift_active_for_keycode(keycode)) {
            if (record->event.pressed) {
                register_code(keycode);
                unregister_code(keycode);

                quickshift_timer_state   = KEY_PRESSED__AWAITING_RELEASE;
                quickshift_timer         = record->event.time;
                quickshift_timer_keycode = keycode;
            } else {
                quickshift_timer_state = INACTIVE__AWAITING_KEYPRESS;
            }
            return true;
        }
    }

    return false;
}

void quickshift__matrix_scan_user(void) {
    if (
        is_quickshift_currently_active() && !is_any_modifier_currently_active()
    ) {
        if (quickshift_timer_state == KEY_PRESSED__AWAITING_RELEASE && timer_elapsed(quickshift_timer) > quickshift_trigger_timer_timeout) {
            register_code(KC_BSPC);
            unregister_code(KC_BSPC);

            quickshift_timer = timer_read();
            quickshift_timer_state = TRIGGERED_BACKSPACE__CHAR_TO_BE_PRESSED_AFTER_DELAY;
        }

        if (quickshift_timer_state == TRIGGERED_BACKSPACE__CHAR_TO_BE_PRESSED_AFTER_DELAY && timer_elapsed(quickshift_timer) > quickshift_char_timer_timeout) {
            bool event_handled = false;

#ifdef KEY_OVERRIDE_ENABLE
            const key_override_t *active_override = find_active_override(quickshift_timer_keycode, MOD_MASK_SHIFT);
            if (active_override) {
                bool should_register_replacement = (active_override->replacement != KC_NO);

                if (active_override->custom_action != NULL) {
                    // Execute custom action: press state.
                    // Returns true if the standard replacement should still be processed.
                    should_register_replacement &= active_override->custom_action(true, active_override->context);

                    // Immediately release the custom action for the tap event
                    active_override->custom_action(false, active_override->context);
                    event_handled = true;
                }

                if (should_register_replacement) {
                    register_code16(active_override->replacement);
                    unregister_code16(active_override->replacement);
                    event_handled = true;
                }
            }
#endif

            if (!event_handled) {
                uint16_t shifted_keycode = get_shifted_keycode(quickshift_timer_keycode);
                register_code16(shifted_keycode);
                unregister_code16(shifted_keycode);
            }

            quickshift_timer = 0;
            quickshift_timer_state = INACTIVE__AWAITING_KEYPRESS;
        }
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
