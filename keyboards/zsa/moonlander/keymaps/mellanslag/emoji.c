#include "action.h"
#include "my_keycodes.h"
#include "unicode/unicode.h"

const struct {
    enum my_keycodes keycode;
    const char* unicode;
} keycode_to_unicode_map[] = {
    {UKC_EMOJI_HERZ, "\u2764\uFE0F"},               // Heart
    {UKC_EMOJI_TRAURIG_STARK, "\u2639\uFE0F"},      // Crying face
    {UKC_EMOJI_KEINE_AHNUNG, "\u200D\u2642\uFE0F"}, // Shrug
    {UKC_EMOJI_MELDEN, "\u200D\u2642\uFE0F"},       // Raising hand
    {UKC_EMOJI_CHECK_MARK, "\u2714\uFE0F"},         // Check mark
    {UKC_EMOJI_ARROW_UP, "\u2B06\uFE0F"},           // Up arrow
    {UKC_EMOJI_ARROW_DOWN, "\u2B07\uFE0F"},         // Down arrow
    {UKC_EMOJI_ARROW_RIGHT, "\u27A1\uFE0F"},        // Right arrow
    {UKC_EMOJI_ARROW_LEFT, "\u2B05\uFE0F"},         // Left arrow
};

static inline const char* get_unicode_string(uint16_t keycode) {
    for (size_t i = 0; i < sizeof(keycode_to_unicode_map)/sizeof(keycode_to_unicode_map[0]); i++) {
        if (keycode_to_unicode_map[i].keycode == keycode) {
            return keycode_to_unicode_map[i].unicode;
        }
    }
    return NULL;
}

bool emoji__process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        const char* unicode = get_unicode_string(keycode);
        if (unicode) {
            send_unicode_string(unicode);
            return false; // Don't process this key normally
        }
    }
    return true; // Process other keys normally
}
