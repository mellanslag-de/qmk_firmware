#pragma once
#include "process_key_override.h"

// Defined in key_overrides.c. QMK's keymap introspection counts this array
// where keymap.c is compiled, so the size is part of the declaration. Bump it
// when adding an override: a count that is too small fails to compile, one that
// is too large only leaves trailing NULL entries, which QMK reads as end of list.
#define KEY_OVERRIDE_COUNT 3
extern const key_override_t *key_overrides[KEY_OVERRIDE_COUNT];
