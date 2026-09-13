#!/usr/bin/env python3
"""Validate handbook RPSC records against canonical school/role notation.

Canonical contract:
  Q[p,k] = Q[POSTECH, KAIST]
  W/B = board roles only
  on a unilateral quiz, the item-gain role must be the role assigned to
  the school whose Q bit is 1.
"""
from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
GAMES = ROOT / "Games.tex"

HEAD_RE = re.compile(
    r"\\gamehead\{(?P<n>\d+)\}"
    r"\{(?P<white_player>[^{}]*)\}"
    r"\{(?P<black_player>[^{}]*)\}"
    r"\{(?P<summary>.*?)\}"
    r"\{(?P<white_team>POSTECH|KAIST)\}"
    r"\{(?P<black_team>POSTECH|KAIST)\}"
    r"\{(?P<date>.*?)\}",
    re.S,
)
BLOCK_RE = re.compile(r"\\begin\{gamerecord\}(.*?)\\end\{gamerecord\}", re.S)
ROW_RE = re.compile(r"^\s*(\d+)\.\s*&\s*Q\[([01]),\s*([01])\]\s*(.*?)\\\\\s*$")
ITEM_RE = re.compile(r"^(?:Timeout\s+)?([WB])\+(Pu|Ro|St)$")


def fail(msg: str) -> None:
    raise AssertionError(msg)


def main() -> None:
    text = GAMES.read_text(encoding="utf-8")
    heads = list(HEAD_RE.finditer(text))
    blocks = list(BLOCK_RE.finditer(text))
    if len(heads) != 6 or len(blocks) != 6:
        fail(f"expected 6 game heads/blocks, got {len(heads)}/{len(blocks)}")

    unilateral = 0
    for idx, (hm, bm) in enumerate(zip(heads, blocks), 1):
        n = int(hm.group("n"))
        if n != idx:
            fail(f"game numbering mismatch: expected {idx}, got {n}")
        wt, bt = hm.group("white_team"), hm.group("black_team")
        if wt == bt or {wt, bt} != {"POSTECH", "KAIST"}:
            fail(f"Game {n}: invalid White/Black school assignment {wt}/{bt}")

        rows = [ln for ln in bm.group(1).splitlines() if ln.strip()]
        if len(rows) != 20:
            fail(f"Game {n}: expected 20 rounds, got {len(rows)}")

        q_postech = q_kaist = 0
        for expected_round, line in enumerate(rows, 1):
            m = ROW_RE.match(line)
            if not m:
                fail(f"Game {n}: malformed row: {line}")
            rnd, p, k, rest = int(m.group(1)), int(m.group(2)), int(m.group(3)), m.group(4).strip()
            if rnd != expected_round:
                fail(f"Game {n}: expected round {expected_round}, got {rnd}")
            q_postech += p
            q_kaist += k

            if p != k:
                unilateral += 1
                im = ITEM_RE.fullmatch(rest)
                if not im:
                    fail(f"Game {n} round {rnd}: unilateral Q must have exactly one item gain, got {rest!r}")
                actual_role = im.group(1)
                correct_school = "POSTECH" if p else "KAIST"
                expected_role = "W" if correct_school == wt else "B"
                if actual_role != expected_role:
                    fail(
                        f"Game {n} round {rnd}: Q[{p}, {k}] means {correct_school} correct, "
                        f"but {actual_role}+ is recorded under WhiteTeam={wt}, BlackTeam={bt}; "
                        f"expected {expected_role}+"
                    )
            else:
                if ITEM_RE.fullmatch(rest):
                    fail(f"Game {n} round {rnd}: equal Q cannot be an item-gain round")
                moves = re.findall(r"(?:Timeout\s+)?[WB][1-4](?:\[[^\]]+\])?:", rest)
                if len(moves) != 2:
                    fail(f"Game {n} round {rnd}: equal Q must contain two board moves, got {len(moves)}")
                if not moves[0].lstrip().startswith("W") and not moves[0].startswith("Timeout W"):
                    fail(f"Game {n} round {rnd}: first move must be White")
                if not moves[1].lstrip().startswith("B") and not moves[1].startswith("Timeout B"):
                    fail(f"Game {n} round {rnd}: second move must be Black")

        # Summary parenthesis stores White--Black quiz score. Compare it with school Q totals
        # after mapping schools to roles.
        sm = re.search(r"\((\d+)--(\d+)\s*\+\s*(\d+)--(\d+)\)", hm.group("summary"))
        if not sm:
            fail(f"Game {n}: cannot parse summary {hm.group('summary')!r}")
        swq, sbq = int(sm.group(1)), int(sm.group(2))
        calc_wq = q_postech if wt == "POSTECH" else q_kaist
        calc_bq = q_postech if bt == "POSTECH" else q_kaist
        if (swq, sbq) != (calc_wq, calc_bq):
            fail(f"Game {n}: summary quiz {swq}-{sbq}, replayed Q gives {calc_wq}-{calc_bq}")

    # Game 3 is historically White=Kim Jungwoo, Black=Hwang Sungjin, with Hwang/POSTECH on Black.
    if (heads[2].group("white_team"), heads[2].group("black_team")) != ("KAIST", "POSTECH"):
        fail("Game 3 school assignment must be White=KAIST, Black=POSTECH")
    game3 = blocks[2].group(1)
    for token in (
        "1.  & Q[0, 1] W+St",
        "3.  & Q[1, 0] B+St",
        "19. & Q[0, 1] W+Ro",
    ):
        if token not in game3:
            fail(f"Game 3 canonical regression token missing: {token}")

    # Direct high-risk regression from the reversed Game 6 assignment.
    game6 = blocks[5].group(1)
    for token in (
        "5.  & Q[1, 0] B+St",
        "6.  & Q[1, 0] B+Ro",
        "7.  & Q[0, 1] W+Pu",
        "11. & Q[1, 0] B+St",
        "19. & Q[0, 1] W+Pu",
    ):
        if token not in game6:
            fail(f"Game 6 canonical regression token missing: {token}")

    print(f"Handbook records: 6 games / 120 rounds / {unilateral} unilateral Q mappings passed")


if __name__ == "__main__":
    main()
