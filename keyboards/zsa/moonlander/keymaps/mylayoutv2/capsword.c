#include "print.h"

#define SHIFT LSFT_T(KC_BSPC)
#define GUI KC_LGUI

bool shift_was_pressed             = false;
bool shift_is_pre_pressed          = false;
bool shift_is_pressed              = false;
bool shift_is_pressed_as_backspace = false;

bool gui_was_pressed = false;
bool gui_is_pressed  = false;

bool other_key_was_pressed = false;

bool finished = false;

#define LOG_DEBUG 2
#define LOG_INFO 1

void reset_state(void) {
    shift_was_pressed             = false;
    shift_is_pre_pressed          = false;
    shift_is_pressed              = false;
    shift_is_pressed_as_backspace = false;

    gui_was_pressed = false;
    gui_is_pressed  = false;

    other_key_was_pressed = false;

    finished = false;

    print("======== RESETTED =========\n");
}

void transitionError(state_t state, extended_state_t extendedState, event_t event) {
    uprintf("[ERROR] Invalid event - state: %u, event: %u", state, event);
}

typedef struct {
    state_t state;
    extended_state_t extendedState;
} transition_return_t;

transition_return_t buildReturn(state_t state, extended_state_t extendedState) {
    return (struct transition_return_t){.state = state, .extendedState = extendedState};
}

typedef struct {
    bool shiftWasPressed, bool guiWasPressed
} extended_state_t;

typedef enum {
    STATE_IDLE,
    STATE_OTHER_KEY_PRESSED,
    STATE_SHIFT_PRESSED,
    STATE_SHIFT_HELD,
    STATE_GUI,
    STATE_SHIFT_PRESSED_GUI,
    STATE_SHIFT_HELD_GUI,
} state_t;

typedef enum {
    EVENT_SHIFT_PRESSED,
    EVENT_SHIFT_HELD,
    EVENT_SHIFT_RELEASED,
    EVENT_GUI_PRESSED,
    EVENT_GUI_RELEASED,
    EVENT_OTHER_KEY_PRESSED,
    EVENT_RESET,
} event_t;



char[] transitionIdle(state_t state, extended_state_t extendedState, event_t event) {
    state_t newState = state;

    switch (event) {
        case EVENT_SHIFT_PRESSED:
            extendedState.shiftWasPressed = true;
            newState = STATE_SHIFT_PRESSED;
            break;
        case EVENT_GUI_PRESSED:
            extendedState.guiWasPressed = true;
            newState = STATE_GUI;
            break;
        default:
            transitionError(state, extendedState, event);
    }

    return buildReturn(newState, extendedState);
}

char[] transitionShiftPressed(state_t state, extended_state_t extendedState, event_t event) {
    state_t newState = state;

    switch (event) {
        case EVENT_SHIFT_HELD:
            bool cond = extendedState.
            newState = STATE_SHIFT_HELD;
            break;
        case EVENT_GUI_PRESSED:
            extendedState.guiWasPressed = true;
            newState = STATE_SHIFT_PRESSED_GUI;
            break;
        default:
            transitionError(state, extendedState, event);
    }

    return buildReturn(newState, extendedState);
}





char[] transition(state_t state, extended_state_t extendedState, event_t event) {
    switch (state) {
        case STATE_IDLE:
            transition_init(state, extendedState, event);
            break;
        case IRRELEVANT:
            transition_irrelevant(state, extendedState, event);
            break;
        default:
            uprintf("[ERROR] Invalid state - state: %u, event: %u", state, event);
    }
}

void state_

    bool
    capsword__pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    finished           = false;
    bool pressed       = record->event.pressed;
    bool release_shift = !pressed && keycode == SHIFT;

    uprintf("pre_process--head - kc: %u, pressed: %s\n", keycode, pressed ? "true" : "false");

    switch (keycode) {
        case SHIFT:
            print("pre_process--SHIFT\n");
            shift_is_pre_pressed = true;
            shift_was_pressed    = true;
            break;
        case GUI:
            print("pre_process--GUI\n");
            gui_is_pressed  = pressed;
            gui_was_pressed = true;
            break;
        default:
            print("pre_process--DEFAULT\n");
            other_key_was_pressed = true;
            break;
    }

    if (release_shift && shift_is_pre_pressed) {
        print("pre_process--mark_shift_as_pressed_as_backspace\n");
        shift_is_pressed_as_backspace = true;
    }

    if (release_shift && shift_is_pre_pressed && gui_was_pressed && !other_key_was_pressed) {
        print("pre_process--enable_caps_word\n");
        caps_word_on();
        reset_state();
        finished = true;
        // dont process shift backspace press action
        return true;
    }

    return false;
}

bool capsword__process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool pressed         = record->event.pressed;
    bool press_gui       = pressed && keycode == GUI;
    bool release_gui     = !pressed && keycode == GUI;
    bool press_shift     = pressed && keycode == SHIFT;
    bool press_other_key = pressed && keycode != GUI && keycode != SHIFT;
    bool ret             = false;

    uprintf("process--head - kc: %u, pressed: %s\n", keycode, pressed ? "true" : "false");

    if (finished || (keycode != SHIFT && keycode != GUI && !shift_was_pressed && !gui_was_pressed)) {
        print("process--aborting\n");
        return ret;
    }

    if (keycode == SHIFT) {
        print("process--shift_pre_pressed__false\n");
        shift_is_pre_pressed = false;
    }

    // dont process short shift presses when We dont want a backspace
    if (press_shift && shift_is_pressed_as_backspace && gui_was_pressed && !other_key_was_pressed) {
        print("process--abort SHIFT-backspace\n");
        ret = true;
    }

    // dont press gui because it may become a combo and then We dont want gui
    if (press_gui) {
        print("process--abort GUI\n");
        ret = true;
    }
    if (press_other_key && gui_was_pressed && !shift_was_pressed) {
        print("process--register_GUI\n");
        register_code(GUI);
    }
    if (release_gui && !shift_was_pressed) {
        print("process--tap_GUI\n");
        tap_code(GUI); // maybe only releasing because its already registered (see above);
    }

    // are We done?
    if (!gui_is_pressed && !shift_is_pressed) {
        print("process--resetting\n");
        if (gui_was_pressed && shift_was_pressed && !other_key_was_pressed) {
            print("process--enable_caps_word\n");
            caps_word_on();
        }
        reset_state();
    }

    return ret;
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
        case DE_UNDS:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case DE_MINS:
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
            return true;
        default:
            return false;
    }
}
