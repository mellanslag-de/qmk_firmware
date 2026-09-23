#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
typedef uint32_t layer_state_t;
typedef struct { bool pressed; uint16_t time; } keyevent_t;
typedef struct { keyevent_t event; } keyrecord_t;
enum { KC_NO = 0, KC_A = 0x04, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M,
       KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S, KC_T, KC_U, KC_V, KC_W, KC_X, KC_Y, KC_Z,
       KC_2 = 0x1F, KC_6 = 0x23, KC_SPC = 0x2C, KC_BSPC = 0x2A, KC_MINS = 0x2D, KC_LBRC = 0x2F,
       KC_SCLN = 0x33, KC_QUOT = 0x34, KC_LEFT_CTRL = 0xE0, KC_LEFT_SHIFT, KC_LEFT_ALT, KC_LEFT_GUI,
       KC_RIGHT_CTRL, KC_RIGHT_SHIFT, KC_RIGHT_ALT, KC_RIGHT_GUI };
#define KC_LCTL KC_LEFT_CTRL
#define KC_LSFT KC_LEFT_SHIFT
#define KC_LALT KC_LEFT_ALT
#define KC_LGUI KC_LEFT_GUI
#define KC_RCTL KC_RIGHT_CTRL
#define KC_RSFT KC_RIGHT_SHIFT
#define KC_RALT KC_RIGHT_ALT
#define KC_RGUI KC_RIGHT_GUI
#define DE_ADIA KC_QUOT
#define DE_ODIA KC_SCLN
#define DE_UDIA KC_LBRC
#define DE_SS   KC_MINS
#define LSFT(kc) ((kc) | 0x0200)
#define MOD_MASK_CTRL  0x11
#define MOD_MASK_SHIFT 0x22
#define MOD_MASK_ALT   0x44
#define MOD_MASK_GUI   0x88
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define IS_MODIFIER_KEYCODE(c) ((c) >= KC_LEFT_CTRL && (c) <= KC_RIGHT_GUI)
extern uint32_t now; extern uint8_t sim_mods; extern layer_state_t layer_state;
void register_code(uint8_t kc); void unregister_code(uint8_t kc);
void register_code16(uint16_t kc); void unregister_code16(uint16_t kc);
static inline uint8_t get_mods(void) { return sim_mods; }
static inline uint8_t get_weak_mods(void) { return 0; }
static inline uint8_t get_oneshot_mods(void) { return 0; }
static inline uint16_t timer_read(void) { return (uint16_t)now; }
static inline uint16_t timer_elapsed(uint16_t t) { return (uint16_t)((uint16_t)now - t); }
static inline void wait_ms(uint32_t ms) { now += ms; }
static inline uint8_t get_highest_layer(layer_state_t s) { uint8_t h = 0; for (uint8_t i = 0; i < 32; i++) if (s & (1UL << i)) h = i; return h; }
