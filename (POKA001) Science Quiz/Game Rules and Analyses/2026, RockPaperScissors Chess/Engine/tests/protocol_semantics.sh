#!/usr/bin/env bash
set -euo pipefail
BIN="${1:-$(dirname "$0")/../build/rpsc-engine}"
out=$(printf 'rpsc\nteams W KAIST B POSTECH\nmatchpk 11 10 6\nshow\nquit\n' | "$BIN")
grep -q 'RPSC Engine 0.22.2' <<<"$out"
grep -q 'teams W KAIST B POSTECH' <<<"$out"
grep -q 'quiz 10-11' <<<"$out"
echo 'Protocol school/board-role boundary regression passed.'
