#include "quickshift.h"
#include <stdbool.h>
#include <stdint.h>
#include "quantum.h"
#include "keymap_german.h"
#include "layers.h"
#ifdef KEY_OVERRIDE_ENABLE
#    include "process_key_override.h"
#    include "keymap_introspection.h"
#endif
// Caps Word is currently switched off. With CAPS_WORD_ENABLE = yes, quantum.h
// provides the declaration and the core builds quantum/caps_word.c; without the
// feature, this stub keeps the key guard below compiling.
#ifndef CAPS_WORD_ENABLE
static inline bool is_caps_word_on(void) {
    return false;
}
#endif

//
// Created by ddeut on 06.07.2021.
//

/*
 * Quickshift: tap a key for the plain character, hold it for the shifted one.
 * The plain character is echoed immediately and corrected once the hold is
 * recognised (backspace + shifted replacement), so typing never waits.
 *
 * Statechart, in XState vocabulary:
 *
 *   QS_IDLE ──KEY_DOWN──▶ QS_ECHOED ──TICK [held > HOLD_TIMEOUT]──▶ QS_CORRECTING
 *      ▲                     │                                          │
 *      └─────KEY_UP──────────┘                                          │
 *      └──────────────────TICK [> CORRECTION_DELAY]─────────────────────┘
 *
 *   state           event     guard                       target
 *   --------------  --------  --------------------------  -------------
 *   any             KEY_DOWN                              QS_ECHOED
 *   any             KEY_UP                                QS_IDLE
 *   any             MOD_DOWN                              QS_IDLE
 *   QS_ECHOED       TICK      held > HOLD_TIMEOUT         QS_CORRECTING
 *   QS_CORRECTING   TICK      elapsed > CORRECTION_DELAY  QS_IDLE
 *
 *   entry QS_IDLE        clear the context
 *   entry QS_ECHOED      tap the key (the immediate echo)
 *   entry QS_CORRECTING  tap backspace, resolve the replacement
 *   exit  QS_CORRECTING  send the replacement, however the state is left
 *
 * Only QS_CORRECTING owes something: the backspace has already removed the
 * echoed character. Its exit action settles that debt on every way out.
 *
 * KEY_DOWN and KEY_UP only reach the machine for quickshift keys and while
 * quickshift_guard_handles_key() holds; TICK only while
 * quickshift_guard_can_progress() holds. Otherwise the event is dropped and
 * the state is kept.
 */

// Runtime switches, read by the machine's guards. They are not part of the context.
static bool is_quickshift_active = true;
static bool is_quickshift_active_at_current_layer = true;

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

typedef enum {
    QS_IDLE,       // nothing pending, nothing owed
    QS_ECHOED,     // plain character is on screen, key still held, clock running
    QS_CORRECTING, // backspace sent, shifted replacement owed
} quickshift_state_t;

typedef enum {
    QS_EV_KEY_DOWN, // quickshift key pressed
    QS_EV_KEY_UP,   // quickshift key released
    QS_EV_MOD_DOWN, // a plain modifier key (Ctrl/Shift/Alt/GUI) pressed
    QS_EV_TICK,     // one matrix scan passed
} quickshift_event_t;

// Data of the current run only, so entering QS_IDLE can wipe it wholesale.
typedef struct {
    uint16_t keycode; // key that started this run
    uint16_t since;   // time of the last state change
#ifdef KEY_OVERRIDE_ENABLE
    const key_override_t *override; // resolved on entry to QS_CORRECTING
#endif
} quickshift_context_t;

static quickshift_state_t   quickshift_state = QS_IDLE;
static quickshift_context_t quickshift_ctx;

// ─── Helpers ─────────────────────────────────────────────────────────────────

static bool is_any_modifier_currently_active(void) {
    uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
    return (mods & (MOD_MASK_CTRL | MOD_MASK_SHIFT | MOD_MASK_ALT | MOD_MASK_GUI)) != 0;
}

static bool is_quickshift_keycode(uint16_t keycode) {
    for (uint8_t i = 0; i < ARRAY_SIZE(quickshift_keycodes); i++) {
        if (quickshift_keycodes[i] == keycode) {
            return true;
        }
    }
    return false;
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
    uint8_t current_layer = get_highest_layer(layer_state);

    for (uint16_t i = 0; i < key_override_count(); i++) {
        const key_override_t *current_override = key_override_get(i);
        if (current_override == NULL) {
            continue;
        }

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

// ─── Guards ──────────────────────────────────────────────────────────────────

static bool quickshift_is_enabled(void) {
    return is_quickshift_active && is_quickshift_active_at_current_layer;
}

// Decides whether a key event reaches the machine at all, and so whether
// quickshift consumes the key.
static bool quickshift_guard_handles_key(uint16_t keycode) {
    return quickshift_is_enabled()
        && !is_caps_word_on()
        && !is_any_modifier_currently_active()
        && is_quickshift_keycode(keycode);
}

// Decides whether TICK may advance the machine. Unlike key events it does not
// look at Caps Word.
static bool quickshift_guard_can_progress(void) {
    return quickshift_is_enabled() && !is_any_modifier_currently_active();
}

// ─── Actions ─────────────────────────────────────────────────────────────────

static void quickshift_tap(uint16_t keycode) {
    register_code(keycode);
    unregister_code(keycode);
}

static void quickshift_tap16(uint16_t keycode) {
    register_code16(keycode);
    unregister_code16(keycode);
}

// Custom actions may block the scan loop for a while: the capital eszett goes
// through unicode input, whose start/finish hooks wait about 125 ms.
static void quickshift_send_replacement(void) {
    uint16_t elapsed = timer_elapsed(quickshift_ctx.since);
    if (elapsed < QUICKSHIFT_CORRECTION_DELAY) {
        wait_ms(QUICKSHIFT_CORRECTION_DELAY - elapsed);
    }

#ifdef KEY_OVERRIDE_ENABLE
    const key_override_t *override = quickshift_ctx.override;
    if (override) {
        bool handled          = false;
        bool send_replacement = override->replacement != KC_NO;

        if (override->custom_action != NULL) {
            // Press state; returns whether the standard replacement should still be sent.
            send_replacement &= override->custom_action(true, override->context);
            // Immediately release the custom action for the tap event
            override->custom_action(false, override->context);
            handled = true;
        }

        if (send_replacement) {
            quickshift_tap16(override->replacement);
            handled = true;
        }

        if (handled) {
            return;
        }
    }
#endif

    quickshift_tap16(LSFT(quickshift_ctx.keycode));
}

// ─── Machine ─────────────────────────────────────────────────────────────────

static void quickshift_exit(void) {
    switch (quickshift_state) {
        case QS_CORRECTING:
            // Skip only while modifiers are held: the replacement would then fire
            // a shortcut instead of typing a character.
            if (!is_any_modifier_currently_active()) {
                quickshift_send_replacement();
            }
            break;
        default:
            break;
    }
}

static void quickshift_enter(quickshift_state_t target) {
    quickshift_state = target;

    switch (target) {
        case QS_IDLE:
            quickshift_ctx = (quickshift_context_t){0};
            break;
        case QS_ECHOED:
            quickshift_tap(quickshift_ctx.keycode);
            break;
        case QS_CORRECTING:
            quickshift_tap(KC_BSPC);
            quickshift_ctx.since = timer_read();
#ifdef KEY_OVERRIDE_ENABLE
            quickshift_ctx.override = find_active_override(quickshift_ctx.keycode, MOD_MASK_SHIFT);
#endif
            break;
    }
}

static void quickshift_transition(quickshift_state_t target) {
    quickshift_exit();
    quickshift_enter(target);
}

static void quickshift_dispatch(quickshift_event_t event, uint16_t keycode, uint16_t time) {
    switch (event) {
        case QS_EV_KEY_DOWN:
            // exit, then assign the new run's context, then entry
            quickshift_exit();
            quickshift_ctx = (quickshift_context_t){.keycode = keycode, .since = time};
            quickshift_enter(QS_ECHOED);
            break;

        case QS_EV_KEY_UP:
        case QS_EV_MOD_DOWN:
            quickshift_transition(QS_IDLE);
            break;

        case QS_EV_TICK:
            if (!quickshift_guard_can_progress()) {
                break;
            }
            if (quickshift_state == QS_ECHOED && timer_elapsed(quickshift_ctx.since) > QUICKSHIFT_HOLD_TIMEOUT) {
                quickshift_transition(QS_CORRECTING);
            } else if (quickshift_state == QS_CORRECTING && timer_elapsed(quickshift_ctx.since) > QUICKSHIFT_CORRECTION_DELAY) {
                quickshift_transition(QS_IDLE);
            }
            break;
    }
}

// ─── QMK entry points ────────────────────────────────────────────────────────

bool quickshift__process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed && IS_MODIFIER_KEYCODE(keycode)) {
        quickshift_dispatch(QS_EV_MOD_DOWN, keycode, record->event.time);
    }

    if (!quickshift_guard_handles_key(keycode)) {
        return false;
    }

    quickshift_dispatch(record->event.pressed ? QS_EV_KEY_DOWN : QS_EV_KEY_UP, keycode, record->event.time);
    return true;
}

void quickshift__matrix_scan_user(void) {
    quickshift_dispatch(QS_EV_TICK, KC_NO, 0);
}

void quickshift__layer_set_state_user(layer_state_t state) {
    uint8_t current_layer = get_highest_layer(state);

    for (uint8_t i = 0; i < ARRAY_SIZE(quickshift_active_layers); i++) {
        if (current_layer == quickshift_active_layers[i]) {
            is_quickshift_active_at_current_layer = true;
            return;
        }
    }
    is_quickshift_active_at_current_layer = false;
}
