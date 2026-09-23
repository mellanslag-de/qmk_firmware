# mellanslag — personal ZSA Moonlander keymap

German-layout (macOS) keymap with quickshift (hold a key for its shifted
character), key overrides, emoji/Unicode input via Karabiner Elements, and
per-layer RGB. The user is new to C; explain C-specific reasoning when it
matters, and keep changes small unless asked otherwise.

## Build and flash

The keyboard is a Moonlander **rev A** (USB PID `0x1969`). Since firmware25
the revision is part of the target:

```sh
qmk compile -kb zsa/moonlander/reva -km mellanslag
qmk flash   -kb zsa/moonlander/reva -km mellanslag   # then press the reset button
```

Output: `zsa_moonlander_reva_mellanslag.bin` in the repo root. A test build
with an extra feature (e.g. `-e CAPS_WORD_ENABLE=yes`) overwrites that file,
so rebuild the regular variant afterwards. Never flash without the user's
go-ahead.

If the qmk CLI reports a missing Python module, install the repo's
`requirements.txt` into the Homebrew qmk venv
(`/opt/homebrew/Cellar/qmk/<version>/libexec/bin/python -m pip install -r requirements.txt`).

## Repository lineage

This is a fork of **ZSA's** fork (`zsa/qmk_firmware`), not of plain QMK.
ZSA deletes most of upstream (other keyboards, docs) and carries its own core
patches (Oryx, RGB, LUFA, `modules/zsa`).

- Work happens on `firmware25`, built on `zsa/firmware25`. `firmware24` is the
  old line (tag `backup-firmware24-premerge`).
- Remotes: `origin` (user's fork `mellanslag-de/qmk_firmware`), `zsa`
  (ZSA fork, the real upstream), `upstream` (`qmk/qmk_firmware`).
- **Never merge `upstream/master` directly**: it conflicts on thousands of
  files ZSA deleted. Update by moving to ZSA's next `firmwareNN` branch and
  cherry-picking the local commits.
- Core change carried locally: `3ab4662591`, a cherry-pick of upstream
  `d8c8872753` (the user's own PR #25886, macOS/Karabiner delay in
  `quantum/process_keycode/process_key_override.c`). Drop it once a ZSA
  branch contains that commit.
- Two git identities appear: `Daniel Deutsch <daniel.deutsch@innoq.com>`
  (local) and `mellanslag-de <github@onlyspam.de>` (GitHub/upstream PRs).

## Files

| File | Role |
|---|---|
| `keymap.c` | Layouts, `unicode_map`, all QMK `*_user` hooks; dispatches to the modules |
| `layers.h` | `enum layers` (LETTERS … EMOJI = 9) |
| `my_keycodes.h` | Custom keycodes from `SAFE_RANGE` (RGB macros, emoji, capital eszett) |
| `keymap.h` | `enum unicode_names`, indices into `unicode_map` (`UM(...)`) |
| `key_overrides.c/.h` | Shift overrides: `2`→`§`, `ß`→`ẞ` (custom action), `6`→`€` |
| `quickshift.c/.h` | Quickshift state machine |
| `emoji.c/.h` | Custom keycode → Unicode string via `send_unicode_string` |
| `rgb.c/.h` | Per-layer `ledmap`, indicator hook, RGB colour macros |
| `keymap.json` | Enables the `zsa/defaults` module (provides `TOGGLE_LAYER_COLOR`, `LED_LEVEL`) |
| `rules.mk` | `UNICODEMAP_ENABLE`, `KEY_OVERRIDE_ENABLE`, one `SRC +=` per module |

### How the files are compiled

Every `.c` file except `keymap.c` is its own translation unit: listed as
`SRC +=` in `rules.mk`, interface in the matching header, everything else
`static`. `keymap.c` includes only headers. Follow this pattern for new code:
a header must not define variables, and a `.c` file is never `#include`d.

Two arrays are special because QMK's keymap introspection is compiled together
with `keymap.c` and takes their size with `ARRAY_SIZE`:

- `unicode_map` therefore stays in `keymap.c`.
- `key_overrides[]` is defined in `key_overrides.c`, but `key_overrides.h`
  declares it with its size, `KEY_OVERRIDE_COUNT`. Bump that when adding an
  override. Too small fails to compile; too large leaves trailing `NULL`
  entries, which QMK treats as end of list.

## Quickshift

Tap a key for the plain character; hold it past `QUICKSHIFT_HOLD_TIMEOUT`
(150 ms) for the shifted one. The plain character is echoed immediately and
corrected afterwards (backspace, then the replacement after
`QUICKSHIFT_CORRECTION_DELAY`, 5 ms), so typing never waits. That
echo-then-correct design is intentional; QMK's Auto Shift avoids the
correction but delays every character.

The code is an explicit state machine in XState vocabulary (the user knows
XState, so keep this wording). The statechart and transition table are the
comment at the top of `quickshift.c`, and they must stay in sync with the
code.

- States: `QS_IDLE`, `QS_ECHOED` (plain char on screen, key held),
  `QS_CORRECTING` (backspace sent, replacement owed).
- Events: `QS_EV_KEY_DOWN`, `QS_EV_KEY_UP`, `QS_EV_MOD_DOWN`, `QS_EV_TICK`.
  The QMK hooks only build events; every state change goes through
  `quickshift_dispatch()`.
- Context `{keycode, since, override}` holds only the current run and is
  wiped on entering `QS_IDLE`. Configuration (timeouts, tables) and runtime
  switches do not belong in it.
- Order on a transition: exit action, then context assignment, then entry
  action.
- The exit action of `QS_CORRECTING` is the only place that sends the
  replacement. It does so on every way out, except while modifiers are held
  (it would fire a shortcut).
- The shifted replacement comes from a matching key override, if any
  (`find_active_override` mirrors QMK's matching), else `LSFT(keycode)`.

Constraints:

- `register_code()` does not re-enter `process_record_user`, so actions may
  send keys without an event queue.
- Actions run inside QMK hooks and block the scan loop. The capital-eszett
  custom action goes through Unicode input (~125 ms of `wait_ms`).
- Mods from mod-taps (e.g. `LSFT_T(KC_BSPC)`) or one-shots do not produce
  `QS_EV_MOD_DOWN`; only plain modifier keycodes do.
- Caps Word is off. The `!is_caps_word_on()` guard is kept on purpose, with a
  stub for when the feature is disabled, so setting `CAPS_WORD_ENABLE = yes`
  works without further changes. Keep that working.

## Unicode and Karabiner Elements

`unicode_input_start()` holds F13 + Left Alt and `unicode_input_finish()`
releases them. Karabiner Elements on the Mac turns that into Unicode hex
input. On the EMOJI layer, `post_process_record_user` holds F19 for
Karabiner. The `wait_ms` in these hooks are timing workarounds for macOS;
do not shorten them without testing on the machine.

## Testing

There are no automated tests in the repo. Quickshift was verified with a
host harness: `quickshift.c` compiled with gcc against minimal QMK stubs,
replaying key sequences with 1 ms ticks and comparing the emitted keys. It
lived in a session scratchpad and is **not** in the repo; rebuild it if
quickshift changes. It ran without `KEY_OVERRIDE_ENABLE`, so the override
path (§, €, ẞ) can only be checked on the real keyboard.

Before any commit: build the regular variant, and for quickshift changes also
`-e CAPS_WORD_ENABLE=yes`.

## Known issues, deliberately left as is

- `keymap.c:202`: `MO(10)` on the QWERTZ_GAMING layer. There is no layer 10
  (the enum ends at `EMOJI` = 9), so the key does nothing. Probably meant `FN`.
- `rgb.c`: the indicator switch handles layers 4–10, so the `ledmap` colours
  for NAVNUM, SYMBOLS and NEO_LAYER_5 never show. Layers without a `ledmap`
  row are guarded against out-of-bounds reads.
- `M_OPEN_DISPLAY_SETTINGS` is bound but not handled anywhere (dead key).
- `QK_DYNAMIC_MACRO_RECORD_STOP` is bound while `DYNAMIC_MACRO_ENABLE` is off.
- `UKC_CAPITAL_ESZETT` exists twice: in `emoji.c` (`send_unicode_string`) and
  as a key override (`register_unicodemap`).
- Quickshift: a non-quickshift key pressed within the 5 ms correction window
  arrives before the replacement.
- Dead code: `set_hsv_color` (`keymap.c`), `is_quickshift_active` is never
  toggled, `UKC_EMOJI_CHECK_MARK` unused, `debug_enable`/`uprintf` leftovers
  in `keyboard_post_init_user` (console is off).

## Working agreements

- Don't change the layout or remove keycodes unless asked.
- Propose before larger rewrites; the user decides.
- Code comments and commit messages in English; the conversation is usually
  German.
