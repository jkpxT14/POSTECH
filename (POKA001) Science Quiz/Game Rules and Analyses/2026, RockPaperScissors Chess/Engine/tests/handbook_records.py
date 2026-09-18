#!/usr/bin/env python3
from pathlib import Path
import re

base = Path(__file__).resolve().parents[2]
text = (base / 'Games.tex').read_text(encoding='utf-8')
heads = re.findall(r'\\gamehead\{(\d+)\}\{(.*?)\}\{(.*?)\}\{(.*?)\}\{(POSTECH|KAIST)\}\{(POSTECH|KAIST)\}\{(.*?)\}', text)
blocks = re.findall(r'\\begin\{gamerecord\}(.*?)\\end\{gamerecord\}', text, flags=re.S)
assert len(heads) == len(blocks) == 11, (len(heads), len(blocks))

rows = 0
for head, block in zip(heads, blocks):
    num, white, black, result_text, white_team, black_team, note = head
    lines = [x.strip() for x in block.splitlines() if re.match(r'^\d+\.\s*&\s*Q\[', x.strip())]
    resigned = '기권' in result_text
    if resigned:
        assert 1 <= len(lines) < 20, f'Game {num}: resignation game must end before Round 20'
    else:
        assert len(lines) == 20, f'Game {num}: {len(lines)} rows'
    assert [int(re.match(r'^(\d+)\.', line).group(1)) for line in lines] == list(range(1, len(lines) + 1)), f'Game {num}: round sequence'
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

# Release-specific anchors for the newly added records.
g10 = heads[9]
g11 = heads[10]
assert g10[1:3] == ('엔진', '선수단') and g10[4:6] == ('KAIST', 'POSTECH') and '29--19' in g10[3]
assert g11[1:3] == ('엔진', '선수단') and g11[4:6] == ('KAIST', 'POSTECH') and '17--9' in g11[3] and '기권' in g11[3]
assert '2026년 9월 17일' in g10[6] and '2026년 9월 17일' in g11[6]
assert len([x for x in blocks[9].splitlines() if re.match(r'^\d+\.', x.strip())]) == 20
assert len([x for x in blocks[10].splitlines() if re.match(r'^\d+\.', x.strip())]) == 11

print(f'Handbook record contract passed: {len(blocks)} games / {rows} quiz rows (Game 11 ends by resignation).')
