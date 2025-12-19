/* Copyright 2020 ZSA Technology Labs, Inc <@zsa>
 * Copyright 2020 Jack Humbert <jack.humb@gmail.com>
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdint.h>
#include "action.h"
#include "info_config.h"
#include "moonlander.h"
#include "key_overrides.c"
#include "quantum.h"
#include QMK_KEYBOARD_H
#include "keymap.h"
#include "keymap_german.h"
#include "quickshift.c"
#include "my_keycodes.h"
#include "emoji.c"
#include "rgb.c"
#include "layers.h"

const uint32_t PROGMEM unicode_map[] = {
    // EMOJIS
    [E_UMARMEN]                    = 0x1F917,  // https://emojipedia.org/hugging-face/
    [UNICODE_KUSS]                 = 0x1F618,  // https://emojipedia.org/face-blowing-a-kiss/
    [UNICODE_VERLIEBT]             = 0x1F970,  // https://emojipedia.org/smiling-face-with-hearts/
    [E_HERZAUGEN]                  = 0x1F60D,  // https://emojipedia.org/smiling-face-with-heart-eyes/
    [E_SMILEY_UMGEDREHT]           = 0x1F643,  // https://emojipedia.org/upside-down-face/
    [E_SMILEY]                     = 0x1F642,  // https://emojipedia.org/slightly-smiling-face/
    [E_SMILEY_OHNE_MUND]           = 0x1F636,  // https://emojipedia.org/face-without-mouth/
    [E_SMILEY_DOOF]                = 0x1F615,  // https://emojipedia.org/confused-face/
    [E_SMILEY_TRAURIG_LEICHT]      = 0x1F641,  // https://emojipedia.org/slightly-frowning-face/
    [E_LAECHELN_MIT_SCHMUNZELN]    = 0x1F604,  // https://emojipedia.org/grinning-face-with-smiling-eyes/
    [E_LAECHELN_MIT_GROSSEN_AUGEN] = 0x1F603,  // https://emojipedia.org/grinning-face-with-big-eyes/
    [E_LAECHELN_MIT_ZAEHNEN]       = 0x1F601,  // https://emojipedia.org/beaming-face-with-smiling-eyes/
    [E_LAECHELN_MIT_GROSSEM_MUND]  = 0x1F600,  // https://emojipedia.org/beaming-face-with-smiling-eyes/
    [E_ZWINKERN]                   = 0x1F609,  // https://emojipedia.org/winking-face/
    [E_LECKEN]                     = 0x1F60B,  // https://emojipedia.org/face-savoring-food/
    [E_ZUNGE]                      = 0x1F61B,  // https://emojipedia.org/face-with-tongue/
    [E_EINE_TRAENE]                = 0x1F605,  // https://emojipedia.org/grinning-face-with-sweat/
    [E_ZWEI_TRAENEN]               = 0x1F602,  // https://emojipedia.org/face-with-tears-of-joy/
    [E_ROTE_WANGEN]                = 0x1F60A,  // https://emojipedia.org/smiling-face-with-smiling-eyes/
    [E_BERECHNEND]                 = 0x1F60F,  // https://emojipedia.org/smirking-face/
    [E_CRAZY]                      = 0x1F92A,  // https://emojipedia.org/zany-face/
    [E_ERSTAUNT]                   = 0x1F632,  // https://emojipedia.org/astonished-face/
    [E_BESORGT]                    = 0x1F633,  // https://emojipedia.org/flushed-face/
    [E_KICHERN]                    = 0x1F92D,  // https://emojipedia.org/face-with-hand-over-mouth/
    [E_AFFE_AUGEN]                 = 0x1F648,  // https://emojipedia.org/see-no-evil-monkey/
    [E_AUGEN_VERDREHEN]            = 0x1F644,  // https://emojipedia.org/face-with-rolling-eyes/
    [E_ROFL]                       = 0x1F923,  // https://emojipedia.org/rolling-on-the-floor-laughing/
    [E_HEULEN]                     = 0x1F62D,  // https://emojipedia.org/loudly-crying-face/
    [E_ENGEL]                      = 0x1F607,  // https://emojipedia.org/smiling-face-with-halo/
    [E_DAUMEN_HOCH]                = 0x1F44D,  // https://emojipedia.org/thumbs-up/
    [E_WINKEN]                     = 0x1F44B,  // https://emojipedia.org/waving-hand/
    [E_FEUER]                      = 0x1F525,  // https://emojipedia.org/fire/
    [E_PARTY_TUETE]                = 0x1F389,  // https://emojipedia.org/party-popper/
    [E_PARTY_TROETE]               = 0x1F973,  // https://emojipedia.org/partying-face/
    [E_CHECK_MARK_BUTTON]          = 0x2705,   // https://emojipedia.org/check-mark-button
    [E_CROSS_MARK]                 = 0x274C,   // https://emojipedia.org/cross-mark/
    [E_FOLDED_HANDS]               = 0x1F64F,  // https://emojipedia.org/folded-hands

    // SPECIAL CHARACTERS
    [C_CAPITAL_ESZETT]             = 0x1E9E,
    [C_CIRCUMFLEX_ACCENT]          = 0x5E,    // https://unicodelookup.com/#94/1
    [C_GRAVE_ACCENT]               = 0x60,    // https://unicodelookup.com/#96/1
    [C_ACUTE_ACCENT]               = 0xB4,    // https://unicodelookup.com/#180/1
    [C_FEMININE_ORD_INDICATOR]     = 0xAA,    // https://unicodelookup.com/#170/1
    [C_MIDDLE_DOT]                 = 0xB7,    // https://unicodelookup.com/#183/1
    [C_EN_DASH]                    = 0x2013,  // https://unicodelookup.com/#8211/1
    [C_POUND_SIGN]                 = 0xA3,    // https://unicodelookup.com/#163/1
    [C_HTS_CHAR_TAB_SET]           = 0x88,    // https://unicodelookup.com/#136/1
    [C_CURRENCY_SIGN]              = 0xA4,    // https://unicodelookup.com/#164/1
    [C_INV_EXCLAMATION_MARK]       = 0xA4,    // https://unicodelookup.com/#161/1
    [C_INV_QUESTION_MARK]          = 0xBF,    // https://unicodelookup.com/#191/1
    [C_SUPERSCRIPT_ONE]            = 0xB9,    // https://unicodelookup.com/#185/1
    [C_CLOCKWISE_ARROW]            = 0x21BB,  // https://unicodelookup.com/#8635/1
    [C_SNGLE_ANGLE_QUOT_MARK_OPEN] = 0x203A,  // https://unicodelookup.com/#8250/1
    [C_SNGLE_ANGLE_QUOT_MARK_CLSE] = 0x2039,  // https://unicodelookup.com/#8249/1
    [C_CENT_SIGN]                  = 0xA2,    // https://unicodelookup.com/#162/1
    [C_YEN_SIGN]                   = 0xA5,    // https://unicodelookup.com/#165/1
    [C_SNGLE_LOW_9_QUOT_MARK_OPEN] = 0x201A,  // https://unicodelookup.com/#8218/1
    [C_LEFT_SINGLE_QUOTATION_MARK] = 0x2018,  // https://unicodelookup.com/#8216/1
    [C_RIGHT_SNGLE_QUOTATION_MARK] = 0x2019,  // https://unicodelookup.com/#8217/1
    [C_LATIN_SMALL_LETTER_LONG_S]  = 0x17F,   // https://unicodelookup.com/#383/1
    [C_NUMERO_SIGN]                = 0x2116,  // https://unicodelookup.com/#8470/1
    [C_MASCULINE_ORD_INDICATOR]    = 0xBA,    // https://unicodelookup.com/#186/1
    [C_HORIZONTAL_ELLIPSIS]        = 0x2026,  // https://unicodelookup.com/#8230/1
    [C_7]                          = 0x37,    // https://unicodelookup.com/#55/1
    [C_EURO_SIGN]                  = 0x20AC,  // https://unicodelookup.com/#8364/1
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // -------
        // @formatter:off
/*
 * ,-------------------------------------------------------------------------------------------------.            ,-------------------------------------------------------------------------------------------------.
 * | ^           | 1           | 2           | 3           | 4           | 5           | Oryx        |            | Print       | 6           | 7           | 8           | 9           | 0           | -           |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * | Tab         | x           | v           | l           | c           | w           | F13         |            | Alt + F4    | k           | h           | g           | f           | q           | Tab         |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * | ß           | u           | i           | a           | e           | o           | `           |            | ´           | s           | n           | r           | t           | d           | y           |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * | Left Ctrl   | ü           | ö           | ä           | p           | z           | %%%%%%%%%%% |            |%%%%%%%%%%%%%| b           | m           | ,           | .           | j           | Right Ctrl  |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * | Ctrl+Win+ ← |AltGr+Sft+Tab| Home / LAlt | ↑ / LAlt    | ↓ / L_Sym   | %%%%%%%%%%% | Esc / LAlt  |            | M_PASSWORD  |%%%%%%%%%%%%%| ← / L_Sum   | → / LAlt    | End / LAlt  | AltGr+Tab   | Ctrl+Win+ → |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * | %%%%%%%%%%% |%%%%%%%%%%%% |%%%%%%%%%%%% | %%%%%%%%%%% | Space       | Backspace   | Application |            | Delete      | Windows/CMD | Enter       | %%%%%%%%%%% |%%%%%%%%%%%% |%%%%%%%%%%%% | %%%%%%%%%%% |
 * | %%%%%%%%%%% | %%%%%%%%%%% | %%%%%%%%%%% | %%%%%%%%%%% | Left FN     | Left Shift  | L_System    |            | Meh         |             | L_NAV       | %%%%%%%%%%% |%%%%%%%%%%%% |%%%%%%%%%%%% | %%%%%%%%%%% |
 * `-------------------------------------------------------------------------------------------------'            `-------------------------------------------------------------------------------------------------'
 */
    [LETTERS] = LAYOUT_moonlander(
//  >                                  <>                                  <>                                  <>                                  <>                                  <>                                  <>                                  <===============>                                  <>                                  <>                                  <>                                  <>                                  <>                                  <>                                  <
    UM(C_CIRCUMFLEX_ACCENT),            KC_1,                               KC_2,                               KC_3,                               KC_4,                               KC_5,                               WEBUSB_PAIR,                                        KC_PSCR,                           KC_6,                               UP(C_7, C_EURO_SIGN),               KC_8,                               KC_9,                               KC_0,                               DE_MINS,
    KC_TAB,                             KC_X,                               KC_V,                               KC_L,                               KC_C,                               KC_W,                               KC_F13,                                             LALT(KC_F4),                       KC_K,                               KC_H,                               KC_G,                               KC_F,                               KC_Q,                               KC_TAB,
    DE_SS,                              KC_U,                               KC_I,                               KC_A,                               KC_E,                               KC_O,                               UM(C_GRAVE_ACCENT),                                 UM(C_ACUTE_ACCENT),                KC_S,                               KC_N,                               KC_R,                               KC_T,                               KC_D,                               DE_Y,
    KC_LGUI,                            DE_UDIA,                            DE_ODIA,                            DE_ADIA,                            KC_P,                               DE_Z,                                                                                                                      KC_B,                               KC_M,                               KC_COMMA,                           KC_DOT,                             KC_J,                               KC_LGUI,
    LCTL(LGUI(KC_LEFT)),                LSFT(RALT(KC_TAB)),                 LT(EMOJI,KC_HOME),                  LALT_T(KC_UP),                      LT(SYMBOLS,KC_DOWN),                                                    LALT_T(KC_ESCAPE),                                  LCTL(LALT(LGUI((KC_X)))),                                              LT(SYMBOLS,KC_LEFT),                LALT_T(KC_RIGHT),                   LT(EMOJI,KC_END),                   RALT(KC_TAB),                       LCTL(LGUI(KC_RIGHT)),
                                                                                                                                                    LT(FN,KC_SPACE),                    LSFT_T(KC_BSPC),                    LT(SYSTEM, KC_APPLICATION),                         MT(MOD_MEH, KC_DELETE),            KC_LCTL,                            LT(NAVNUM,KC_ENTER)
  ),
  [NAVNUM] = LAYOUT_moonlander(
/*
 * L_<Layer_Name>
 *
 * ,-------------------------------------------------------------------------------------------------.            ,-------------------------------------------------------------------------------------------------.
 * |             |             |             |             |             |             |             |            |             |             |             |             | Num /       | Num *       |  Num -      |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * |             | Page Up     | Backspace   | ↑           | Delete      | Page Down   |             |            |             | ¡ (0161)    | 7           | 8           | 9           | Num +       |             |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * |             | Home        | ←           | ↓           | →           | End         | `           |            |             | 0           | 4           | 5           | 6           | Num ,       | .           |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * |             | Escape      | Tab         | Insert      | Enter       | Undo        | %%%%%%%%%%% |            |%%%%%%%%%%%%%| :           | 1           | 2           | 3           | ;           |             |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * |             |             | Cut         | Copy        | Paste       | %%%%%%%%%%% |             |            |             |%%%%%%%%%%%%%|             |             |             |             |             |
 * |-------------+-------------+-------------+-------------+-------------+-------------+-------------|            |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
 * | %%%%%%%%%%% | %%%%%%%%%%% | %%%%%%%%%%% | %%%%%%%%%%% |             |             |             |            |             |             |             |%%%%%%%%%%%%%|%%%%%%%%%%%%%|%%%%%%%%%%%%%|%%%%%%%%%%%%%|
 * `-------------------------------------------------------------------------------------------------'            `-------------------------------------------------------------------------------------------------'
 */
//  >                                  <>                                  <>                                  <>                                  <>                                  <>                                  <>                                  <===============>                                  <>                                  <>                                  <>                                  <>                                  <>                                  <>                                  <
    KC_TRANSPARENT,                     UM(C_FEMININE_ORD_INDICATOR),       UM(C_MASCULINE_ORD_INDICATOR),      UM(C_NUMERO_SIGN),                  KC_TRANSPARENT,                     UM(C_MIDDLE_DOT),                   KC_TRANSPARENT,                                    KC_TRANSPARENT,                     UM(C_POUND_SIGN),                   UM(C_CURRENCY_SIGN),                UM(C_HTS_CHAR_TAB_SET),             KC_KP_SLASH,                        KC_KP_ASTERISK,                     KC_KP_MINUS,
    KC_TRANSPARENT,                     KC_PGUP,                            KC_BSPC,                            KC_UP,                              KC_DELETE,                          KC_PGDN,                            KC_TRANSPARENT,                                    KC_TRANSPARENT,                     UM(C_INV_EXCLAMATION_MARK),         KC_7,                               KC_8,                               KC_9,                               KC_KP_PLUS,                         KC_TRANSPARENT,
    UM(C_EN_DASH),                      KC_HOME,                            KC_LEFT,                            KC_DOWN,                            KC_RIGHT,                           KC_END,                             KC_TRANSPARENT,                                    UM(C_INV_QUESTION_MARK),            KC_0,                               KC_4,                               KC_5,                               KC_6,                               KC_KP_DOT,                          KC_DOT,
    KC_TRANSPARENT,                     KC_ESCAPE,                          KC_TAB,                             KC_INSERT,                          KC_ENTER,                           KC_UNDO,                                                                                                                   LSFT(KC_DOT),                       KC_1,                               KC_2,                               KC_3,                               LSFT(KC_COMMA),                     KC_TRANSPARENT,
    KC_TRANSPARENT,                     KC_TRANSPARENT,                     LCTL(KC_X),                         LCTL(KC_C),                         LCTL(KC_V),                                                             KC_TRANSPARENT,                                    KC_TRANSPARENT,                                                         KC_TRANSPARENT,                     KC_TRANSPARENT,                     KC_TRANSPARENT,                     KC_TRANSPARENT,                     KC_TRANSPARENT,
                                                                                                                                                    KC_TRANSPARENT,                     KC_TRANSPARENT,                     KC_TRANSPARENT,                                    KC_TRANSPARENT,                     KC_TRANSPARENT,                     KC_TRANSPARENT
  ),
  [SYMBOLS] = LAYOUT_moonlander(
//  >                                  <>                                  <>                                  <>                                  <>                                   <>                                   <>                                  <===============>                                   <>                                  <>                                  <>                                   <>                               <>                               <>                              <
    UM(C_CLOCKWISE_ARROW),          UM(C_SUPERSCRIPT_ONE),          RALT(KC_2),                     RALT(KC_3),                     UM(C_SNGLE_ANGLE_QUOT_MARK_OPEN),UM(C_SNGLE_ANGLE_QUOT_MARK_CLSE),KC_TRANSPARENT,                                 KC_TRANSPARENT,                 UM(C_CENT_SIGN),                UM(C_YEN_SIGN),                 UM(C_SNGLE_LOW_9_QUOT_MARK_OPEN),UM(C_LEFT_SINGLE_QUOTATION_MARK),UM(C_RIGHT_SNGLE_QUOTATION_MARK),KC_TRANSPARENT,
    KC_TRANSPARENT,                 UM(C_HORIZONTAL_ELLIPSIS),      DE_UNDS,                        RALT(KC_5),                     RALT(KC_6),                      DE_CIRC,                         KC_TRANSPARENT,                                 KC_TRANSPARENT,                 LSFT(KC_1),                     DE_LABK,                        LSFT(DE_LABK),                   LSFT(KC_0),                      LSFT(KC_6),                      KC_TRANSPARENT,
    UM(C_LATIN_SMALL_LETTER_LONG_S),RALT(LSFT(KC_7)),               LSFT(KC_7),                     RALT(KC_8),                     RALT(KC_9),                      LSFT(DE_PLUS),                   KC_TRANSPARENT,                                 KC_TRANSPARENT,                 LSFT(DE_SS),                    LSFT(KC_8),                     LSFT(KC_9),                      DE_MINS,                         LSFT(KC_DOT),                    RALT(KC_Q),
    KC_TRANSPARENT,                 DE_HASH,                        LSFT(KC_4),                     RALT(KC_7),                     RALT(KC_N),                   LSFT(DE_ACUT),                                                                                                           DE_PLUS,                        LSFT(KC_5),                     LSFT(KC_2),                      LSFT(DE_HASH),               LSFT(KC_COMMA),              KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 MO(EMOJI),                                                            KC_TRANSPARENT,                                 KC_TRANSPARENT,                                                     MO(EMOJI),                      KC_TRANSPARENT,                  KC_TRANSPARENT,              KC_TRANSPARENT,              KC_TRANSPARENT,
                                                                                                                                                    KC_TRANSPARENT,                  KC_TRANSPARENT,                  KC_TRANSPARENT,                                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT
  ),
  [NEO_LAYER_5] = LAYOUT_moonlander(
//  >                                  <>                              <>                              <>                              <>                              <>                              <>                              <===============>                              <>                              <>                              <>                              <>                              <>                              <>                              <
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 LCTL(LGUI(LSFT(RALT(KC_B)))),   KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                                                                                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                                                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
                                                                                                                                    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT
  ),
  [NEO_LAYER_6] = LAYOUT_moonlander(
//  >                              <>                              <>                              <>                              <>                              <>                              <>                              <===============>                              <>                              <>                              <>                              <>                              <>                              <>                              <
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                                                                                 KC_TRANSPARENT,                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                                                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,
                                                                                                                                    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                 KC_TRANSPARENT,                 KC_TRANSPARENT,                KC_TRANSPARENT
  ),
  [QWERTZ] = LAYOUT_moonlander(
//  >                              <>                              <>                              <>                              <>                              <>                              <>                              <===============>                              <>                              <>                              <>                              <>                              <>                              <>                              <
    DE_CIRC,                        KC_1,                           KC_2,                           KC_3,                           KC_4,                           KC_5,                           DE_ACUT,                                        DE_PLUS,                       KC_6,                           KC_7,                           KC_8,                           KC_9,                           KC_0,                           DE_SS,
    KC_TAB,                         KC_Q,                           KC_W,                           KC_E,                           KC_R,                           KC_T,                           KC_INSERT,                                      DE_HASH,                       DE_Z,                           KC_U,                           KC_I,                           KC_O,                           KC_P,                           DE_UDIA,
    KC_CAPS_LOCK,                   KC_A,                           KC_S,                           KC_D,                           KC_F,                           KC_G,                           KC_HOME,                                        KC_END,                        KC_H,                           KC_J,                           KC_K,                           KC_L,                           DE_ODIA,                        DE_ADIA,
    KC_LSFT,                        DE_Y,                           KC_X,                           KC_C,                           KC_V,                           KC_B,                                                                                                          KC_N,                           KC_M,                           KC_COMMA,                       KC_DOT,                         DE_MINS,                        KC_RSFT,
    KC_LCTL,                        KC_LGUI,                        KC_LALT,                        KC_UP,                          KC_DOWN,                                                        KC_ESCAPE,                                      KC_APPLICATION,                                                KC_LEFT,                        KC_RIGHT,                       KC_RALT,                        MO(FN),                         KC_RCTL,
                                                                                                                                    KC_SPACE,                       KC_BSPC,                      KC_PGUP,                                        KC_PGDN,                     KC_DELETE,                      KC_ENTER
  ),
  [QWERTZ_GAMING] = LAYOUT_moonlander(
//  >                              <>                              <>                              <>                              <>                              <>                              <>                              <===============>                              <>                              <>                              <>                              <>                              <>                              <>                              <
    DE_CIRC,                        KC_1,                           KC_2,                           KC_3,                           KC_4,                           KC_5,                           DE_ACUT,                                        DE_PLUS,                       KC_6,                           KC_7,                           KC_8,                           KC_9,                           KC_0,                           DE_SS,
    KC_TAB,                         KC_Q,                           KC_W,                           KC_E,                           KC_R,                           KC_T,                           KC_INSERT,                                      DE_HASH,                       KC_Z,                           KC_U,                           KC_I,                           KC_O,                           KC_P,                           DE_UDIA,
    KC_CAPS_LOCK,                   KC_A,                           KC_S,                           KC_D,                           KC_F,                           KC_G,                           KC_HOME,                                        KC_END,                        KC_H,                           KC_J,                           KC_K,                           KC_L,                           DE_ODIA,                        DE_ADIA,
    KC_LSFT,                        KC_Y,                           KC_X,                           KC_C,                           KC_V,                           KC_B,                                                                                                          KC_N,                           KC_M,                           KC_COMMA,                       KC_DOT,                         DE_MINS,                        KC_RSFT,
    KC_LCTL,                        KC_LGUI,                        KC_LALT,                        KC_UP,                          KC_SPACE,                                                       KC_ESCAPE,                                      KC_APPLICATION,                                                KC_LEFT,                        KC_RIGHT,                       KC_RALT,                        MO(10),                         KC_RCTL,
                                                                                                                                    KC_DOWN,                        KC_BSPC,                        KC_PGUP,                                        KC_PGDN,                      KC_DELETE,                     KC_ENTER
),
  [FN] = LAYOUT_moonlander(
//  >                                      <>                                  <>                                  <>                                  <>                                  <>                                  <>                              <===============>                              <>                              <>                              <>                              <>                              <>                              <>                              <
    M_OPEN_DISPLAY_SETTINGS,        UKC_CAPITAL_ESZETT,                          KC_AUDIO_VOL_DOWN,              KC_AUDIO_MUTE,                  KC_AUDIO_VOL_UP,                MU_TOGG,                        QK_BOOTLOADER,                                 RGB_SAD,                        RGB_SAI,                        KC_MEDIA_PREV_TRACK,            KC_MEDIA_PLAY_PAUSE,            KC_MEDIA_NEXT_TRACK,            KC_MEDIA_REWIND,                KC_MEDIA_FAST_FORWARD,
    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_MS_WH_LEFT,                  KC_MS_UP,                       KC_MS_WH_RIGHT,                 QK_DYNAMIC_MACRO_PLAY_1,        QK_DYNAMIC_MACRO_RECORD_START_1,               RGB_HUD,                        RGB_HUI,                        KC_F7,                          KC_F8,                          KC_F9,                          KC_F10,                         KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_MS_WH_UP,                    KC_MS_LEFT,                     KC_MS_DOWN,                     KC_MS_RIGHT,                    QK_DYNAMIC_MACRO_PLAY_2,        QK_DYNAMIC_MACRO_RECORD_START_2,               RGB_TOG,                        RGB_VAI,                        KC_F4,                          KC_F5,                          KC_F6,                          KC_F11,                         KC_TRANSPARENT,
    KC_TRANSPARENT,                 KC_MS_WH_DOWN,                  LALT(LCTL(LSFT(KC_1))),         LALT(LCTL(LSFT(KC_2))),         LALT(LCTL(LSFT(KC_3))),         QK_DYNAMIC_MACRO_RECORD_STOP,                                                                                          RGB_TOG,                        KC_F1,                          KC_F2,                      KC_F3,                      KC_F12,                         TO(QWERTZ_GAMING),
    TO(LETTERS),                    KC_TRANSPARENT,                 M_RGB_YELLOW,                   M_RGB_ORANGE,                   M_RGB_WHITE,                                                        KC_TRANSPARENT,                                LCTL(LSFT(KC_X)),                                                   KC_MS_BTN1,                     KC_MS_BTN2,                 KC_MS_BTN3,                 KC_TRANSPARENT,                 TO(QWERTZ),
                                                                                                                                                    KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT,                                KC_TRANSPARENT,                 KC_TRANSPARENT,                 KC_TRANSPARENT
  ),
  [EMOJI] = LAYOUT_moonlander(
//  >                              <>                              <>                              <>                              <>                              <>                              <>                              <===============>                              <>                              <>                              <>                              <>                              <>                              <>                              <
    KC_NO,                          UKC_EMOJI_ARROW_LEFT,           UKC_EMOJI_ARROW_DOWN,           UKC_EMOJI_ARROW_UP,             UKC_EMOJI_ARROW_RIGHT,          KC_NO         ,                 KC_NO,                                         KC_NO,                          KC_NO,                          KC_NO,                          UM(E_FOLDED_HANDS),       KC_NO,                           KC_NO,                          KC_NO,
    UM(E_FEUER),                    UKC_EMOJI_KEINE_AHNUNG,         UKC_EMOJI_MELDEN,               UM(E_AFFE_AUGEN),               UM(E_DAUMEN_HOCH),              UM(E_WINKEN),                   UM(E_HEULEN),                                  UM(E_PARTY_TUETE),              UM(E_ENGEL),                    UM(E_SMILEY_UMGEDREHT),         UM(E_SMILEY),                   UM(E_ZWINKERN),                  UM(E_BERECHNEND),               UM(E_CROSS_MARK),
    UM(E_KICHERN),                  UM(E_HERZAUGEN),                UM(UNICODE_VERLIEBT),           UM(UNICODE_KUSS),               UKC_EMOJI_HERZ,                 UM(E_ROTE_WANGEN),              UM(E_ERSTAUNT),                                UM(E_PARTY_TROETE),             UM(E_HEULEN),                   UM(E_LAECHELN_MIT_SCHMUNZELN),  UM(E_LAECHELN_MIT_ZAEHNEN),     UM(E_LAECHELN_MIT_GROSSEN_AUGEN),UM(E_LAECHELN_MIT_GROSSEM_MUND),UM(E_CHECK_MARK_BUTTON),
    UKC_EMOJI_TRAURIG_STARK,        UM(E_SMILEY_TRAURIG_LEICHT),    UM(E_SMILEY_OHNE_MUND),         UM(E_SMILEY_DOOF),              UM(E_AUGEN_VERDREHEN),          UM(E_BESORGT),                                                                                                         UM(E_ROFL),                     UM(E_ZWEI_TRAENEN),             UM(E_EINE_TRAENE),              UM(E_ZUNGE),                 UM(E_LECKEN),               UM(E_CRAZY),
    KC_NO,                          KC_NO,                          KC_NO,                          KC_NO,                          KC_NO,                                                              KC_NO,                                         KC_NO,                                                              KC_NO,                          KC_NO,                          KC_NO,                       KC_NO,                      KC_NO,
                                                                                                                                                    KC_NO,                          KC_NO,                          KC_NO,                                         KC_NO,                          KC_NO,                          KC_NO
  ),
};
// @formatter:on

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
    debug_enable = true;

    uprintf("KC_LGUI: %u\n", KC_LGUI);
    uprintf("KC_LSFT: %u\n", KC_LSFT);
    uprintf("KC_A: %u\n", KC_A);
    uprintf("LSFT_T(KC_BSPC): %u\n", LSFT_T(KC_BSPC));
}

bool rgb_matrix_indicators_user(void) {
    rgb__rgb_matrix_indicators_user();
    return true;
}

void set_hsv_color(keyrecord_t *record, int h, int s, int v) {
    if (record->event.pressed) {
        rgblight_mode(1);
        rgblight_sethsv(h, s, v);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!rgb__process_record_user(keycode, record)) {
        return false;
    }

    if (!emoji__process_record_user(keycode, record)) {
        return false;
    }

    if (quickshift__process_record_user(keycode, record)) {
        return false;
    }

    return true;
}

void matrix_scan_user() {
    /*    if (mouse_delay_active && timer_elapsed(quickshift_timer) > 250) {
            mouse_delay_active = false;
            register_code(mouse_delay_keycode);
        }*/
    quickshift__matrix_scan_user();
}

// Override QMK Unicode start/finish hooks (these are weak in quantum/unicode/unicode.c).
// Start: Ctrl+Cmd+Space (macOS Emoji picker), Finish: Enter.
void unicode_input_start(void) {
    register_code(KC_F13);
    register_code(KC_LALT);
    wait_ms(25);
}

void unicode_input_finish(void) {
    unregister_code(KC_LALT);
    unregister_code(KC_F13);
    wait_ms(100);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    quickshift__layer_set_state_user(state);
    return state;
}

void suspend_power_down_user(void)
{
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_kb(void)
{
    rgb_matrix_set_suspend_state(false);
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t currentLayer = biton32(layer_state);
    if (currentLayer == EMOJI) {
        register_code(KC_F19);
    } else {
        unregister_code(KC_F19);
    }
}
