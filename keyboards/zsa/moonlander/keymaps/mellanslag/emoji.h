#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "action.h"

// Sends the Unicode string mapped to an emoji keycode. Returns false when the key was handled.
bool emoji__process_record_user(uint16_t keycode, keyrecord_t *record);
