#!/bin/sh
# Builds quickshift.c against the QMK stubs on the host and compares the
# emitted keys of every scenario with expected.txt.
#   ./run.sh           run and diff
#   ./run.sh --update  accept the current output as the new expectation
set -eu
here=$(cd "$(dirname "$0")" && pwd)
keymap="$here/../.."
bin=$(mktemp "${TMPDIR:-/tmp}/quickshift-test.XXXXXX")
trap 'rm -f "$bin"' EXIT

cc -std=gnu11 -Wall -Wextra -Wno-unused-parameter \
    -I"$here/stubs" -I"$here/.." -I"$keymap" \
    "$here/driver.c" "$here/../quickshift.c" -o "$bin"

if [ "${1:-}" = "--update" ]; then
    "$bin" > "$here/expected.txt"
    echo "expected.txt updated"
elif "$bin" | diff -u "$here/expected.txt" -; then
    echo "quickshift: all scenarios match"
else
    echo "quickshift: output differs from expected.txt" >&2
    exit 1
fi
