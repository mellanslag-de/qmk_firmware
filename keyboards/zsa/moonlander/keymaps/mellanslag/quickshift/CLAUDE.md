# Quickshift

Tap a key for the plain character; hold it past `QUICKSHIFT_HOLD_TIMEOUT`
(150 ms) for the shifted one. The plain character is echoed immediately and
corrected afterwards (backspace, then the replacement after
`QUICKSHIFT_CORRECTION_DELAY`, 5 ms), so typing never waits. That
echo-then-correct design is intentional; QMK's Auto Shift avoids the
correction but delays every character.

## Files

| File | Role |
|---|---|
| `quickshift.h` | Interface: the three `quickshift__*` entry points and the timeout macros (overridable in `config.h`) |
| `quickshift.c` | The state machine; everything else is `static` |
| `test/` | Host harness, see below |

The keymap builds it via `SRC += quickshift/quickshift.c` in `../rules.mk` and
includes `quickshift/quickshift.h` in `../keymap.c`. The folder is not fully
standalone: `quickshift.c` includes `layers.h` and `keymap_german.h` from the
keymap, because the keycode list and the active layers are hard-coded in it.

## State machine

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

## Constraints

- `register_code()` does not re-enter `process_record_user`, so actions may
  send keys without an event queue.
- Actions run inside QMK hooks and block the scan loop. The capital-eszett
  custom action goes through Unicode input (~125 ms of `wait_ms`).
- Mods from mod-taps (e.g. `LSFT_T(KC_BSPC)`) or one-shots do not produce
  `QS_EV_MOD_DOWN`; only plain modifier keycodes do.
- Caps Word is off. The `!is_caps_word_on()` guard is kept on purpose, with a
  stub for when the feature is disabled, so setting `CAPS_WORD_ENABLE = yes`
  works without further changes. Keep that working.

## Testing

```sh
./test/run.sh           # build on the host, compare with test/expected.txt
./test/run.sh --update  # accept the current output as the new expectation
```

`test/driver.c` replays key sequences with 1 ms scan ticks against
`quickshift.c`, compiled with the host `cc` and minimal QMK stand-ins in
`test/stubs/`. Each scenario prints what the machine sent (`<BS>` is
backspace, `[x]` a key passed through to QMK). Run it after every change and
review any diff before `--update`: a changed line is a behaviour change.

The stubs do not define `KEY_OVERRIDE_ENABLE`, so the override path (§, €,
ẞ) is not covered; check it on the real keyboard. Nothing in `test/` is part
of the firmware build.

Before committing a quickshift change, also build the firmware both plain
and with `-e CAPS_WORD_ENABLE=yes`.

## Known issues

- A non-quickshift key pressed within the 5 ms correction window arrives
  before the replacement (scenario 11 in the harness).
- `is_quickshift_active` is never toggled; there is no runtime switch yet.
