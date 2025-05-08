#include "quantum.h"
#include "process_key_override.h"


static const key_override_t n_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_N, LSFT(KC_O));

const key_override_t **key_overrides = (const key_override_t *[]){
    &n_key_override,
    NULL,
};
