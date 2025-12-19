#include "quantum.h"
#include "keymap_german.h"
#include "process_key_override.h"
#include "process_unicodemap.h"
#include "keymap.h"
#include "layers.h"

// Forward declaration of your emoji processor or just use send_unicode_string directly
extern void send_unicode_string(const char *str);

static const key_override_t shifted_2_becomes_paragraph_sign = ko_make_basic(MOD_MASK_SHIFT, KC_2, LSFT(KC_3));

static bool handle_capital_eszett(bool pressed, void *context) {
	if (pressed) {
        register_unicodemap(C_CAPITAL_ESZETT);
    }
	return true;
}
static const key_override_t shifted_eszet_becomes_large_eszet = {
	.trigger = DE_SS,
	.trigger_mods = MOD_MASK_SHIFT,
    .layers = (1 << LETTERS),
    .negative_mod_mask = ~MOD_MASK_SHIFT,
    .suppressed_mods = ~MOD_MASK_SHIFT,
	.replacement = KC_NO,
    .options = ko_options_default,
    .custom_action = handle_capital_eszett,
    .context = NULL,
	.enabled = NULL
};

static const key_override_t shifted_6_becomes_euro_sign = {
	.trigger = KC_6,
	.trigger_mods = MOD_MASK_SHIFT,
    .layers = (1 << LETTERS),
    .negative_mod_mask = ~MOD_MASK_SHIFT,
    .suppressed_mods = MOD_MASK_SHIFT,
	.replacement = LALT(KC_E),
    .options = ko_options_all_activations,
    .custom_action = NULL,
    .context = NULL,
	.enabled = NULL
};
const key_override_t **key_overrides = (const key_override_t *[]){
    &shifted_2_becomes_paragraph_sign,
	&shifted_eszet_becomes_large_eszet,
	&shifted_6_becomes_euro_sign,
    NULL,
};