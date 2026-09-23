// Host-side test driver for quickshift: replays key sequences with 1 ms scan
// ticks and prints what the machine sends. See ../CLAUDE.md.
#include "qmk_stub.h"
uint32_t now = 1000; uint8_t sim_mods = 0; layer_state_t layer_state = 1; /* LETTERS */
#include "quickshift.h"
static char out[512]; static int n;
static const char *name(uint16_t kc) {
    static char b[16]; uint16_t base = kc & 0xFF; char c;
    if (base >= KC_A && base <= KC_Z) c = 'a' + (base - KC_A);
    else if (base == KC_SPC) return "_"; else if (base == KC_BSPC) return "<BS>";
    else if (base == KC_2) c = '2'; else if (base == KC_6) c = '6'; else if (base == KC_MINS) c = 's';
    else if (base >= KC_LEFT_CTRL) return "<MOD>"; else c = '?';
    snprintf(b, sizeof b, "%c", (kc & 0x0200) ? c - 32 * (c >= 'a') : c); return b;
}
void register_code(uint8_t kc) { n += snprintf(out + n, sizeof out - n, "%s", name(kc)); }
void unregister_code(uint8_t kc) {}
void register_code16(uint16_t kc) { n += snprintf(out + n, sizeof out - n, "%s", name(kc)); }
void unregister_code16(uint16_t kc) {}
static void key(uint16_t kc, bool down) {
    keyrecord_t r = {{down, (uint16_t)now}};
    if (!quickshift__process_record_user(kc, &r) && down) n += snprintf(out + n, sizeof out - n, "[%s]", name(kc)); /* passthrough */
}
static void run_to(uint32_t t) { while (now < t) { now++; quickshift__matrix_scan_user(); } }
#define AT(t) run_to(start + (t))
static void scenario(const char *title, void (*f)(uint32_t)) {
    n = 0; out[0] = 0; sim_mods = 0; layer_state = 1; quickshift__layer_set_state_user(layer_state);
    uint32_t start = now; f(start); run_to(start + 1000); printf("%-44s %s\n", title, out); now += 1000;
}
static void s1(uint32_t start) { key(KC_A,1); AT(50); key(KC_A,0); }
static void s2(uint32_t start) { key(KC_A,1); AT(300); key(KC_A,0); }
static void s3(uint32_t start) { key(KC_A,1); AT(153); key(KC_A,0); }
static void s4(uint32_t start) { key(KC_A,1); AT(153); key(KC_LSFT,1); sim_mods=0x02; AT(200); key(KC_LSFT,0); sim_mods=0; key(KC_A,0); }
static void s5(uint32_t start) { key(KC_A,1); AT(153); key(KC_B,1); AT(200); key(KC_B,0); key(KC_A,0); }
static void s6(uint32_t start) { key(KC_A,1); AT(50); key(KC_B,1); AT(80); key(KC_A,0); AT(300); key(KC_B,0); }
static void s7(uint32_t start) { key(KC_A,1); AT(100); key(KC_LCTL,1); sim_mods=0x01; AT(300); key(KC_LCTL,0); sim_mods=0; key(KC_A,0); }
static void s8(uint32_t start) { sim_mods=0x02; key(KC_A,1); AT(300); key(KC_A,0); }
static void s9(uint32_t start) { layer_state=1<<1; quickshift__layer_set_state_user(layer_state); key(KC_A,1); AT(300); key(KC_A,0); }
static void s10(uint32_t start) { key(KC_A,1); AT(153); sim_mods=0x01; AT(160); key(KC_A,0); AT(400); sim_mods=0; }
static void s11(uint32_t start) { key(KC_A,1); AT(153); key(KC_SPC,1); key(KC_SPC,0); AT(300); key(KC_A,0); }
static void s12(uint32_t start) { key(KC_A,1); AT(160); key(KC_SPC,1); key(KC_SPC,0); AT(300); key(KC_A,0); }
static void s13(uint32_t start) { key(DE_SS,1); AT(300); key(DE_SS,0); key(KC_2,1); AT(40); key(KC_2,0); }
int main(void) {
    scenario("01 tap a", s1);
    scenario("02 hold a 300ms", s2);
    scenario("03 release inside correction window", s3);
    scenario("04 Shift pressed inside correction window", s4);
    scenario("05 other qs key inside correction window", s5);
    scenario("06 rollover a->b, a released early", s6);
    scenario("07 Ctrl pressed while echoed", s7);
    scenario("08 Shift already held (passthrough)", s8);
    scenario("09 inactive layer (passthrough)", s9);
    scenario("10 mod-tap mods active, key up in window", s10);
    scenario("11 non-qs key inside correction window", s11);
    scenario("12 non-qs key after correction", s12);
    scenario("13 eszett hold, then tap 2", s13);
}
