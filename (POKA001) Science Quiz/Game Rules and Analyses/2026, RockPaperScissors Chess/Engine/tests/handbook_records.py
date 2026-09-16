#!/usr/bin/env python3
from pathlib import Path
import re

base = Path(__file__).resolve().parents[2]
text = (base / 'Games.tex').read_text(encoding='utf-8')
heads = re.findall(r'\\gamehead\{(\d+)\}\{.*?\}\{.*?\}\{.*?\}\{(POSTECH|KAIST)\}\{(POSTECH|KAIST)\}\{', text)
blocks = re.findall(r'\\begin\{gamerecord\}(.*?)\\end\{gamerecord\}', text, flags=re.S)
assert len(heads) == len(blocks) == 9, (len(heads), len(blocks))

rows = 0
for (num, white_team, black_team), block in zip(heads, blocks):
    lines = [x.strip() for x in block.splitlines() if re.match(r'^\d+\.\s*&\s*Q\[', x.strip())]
    assert len(lines) == 20, f'Game {num}: {len(lines)} rows'
    for line in lines:
        rows += 1
        m = re.search(r'Q\[(\d),\s*(\d)\]', line)
        assert m
        p, k = map(int, m.groups())  # fixed P/K order
        gains = re.findall(r'\b([WB])\+(Pu|Ro|St)\b', line)
        if p != k:
            assert len(gains) == 1, f'Game {num}: solo-correct row must have one item gain: {line}'
            role = gains[0][0]
            correct_school = 'POSTECH' if p else 'KAIST'
            expected_role = 'W' if white_team == correct_school else 'B'
            assert role == expected_role, f'Game {num}: P/K -> W/B mismatch: {line}'
        else:
            assert not gains, f'Game {num}: equal quiz result cannot grant item: {line}'

print(f'Handbook record contract passed: {len(blocks)} games / {rows} quiz rows.')
