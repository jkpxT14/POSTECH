"""Verify release files and preserved structure (Python 3 standard library)."""
from pathlib import Path
import hashlib
import json
import re

root = Path(__file__).resolve().parent.parent
notes = root / 'ReleaseNotes'
manifest = json.loads((notes / 'SHA256.json').read_text())
for name, expected in manifest.items():
    path = root / name
    assert path.is_file(), f'Missing file: {name}'
    assert hashlib.sha256(path.read_bytes()).hexdigest() == expected, f'Changed file: {name}'
baseline = json.loads((notes / 'BASELINE.json').read_text())
for name, expected in baseline['structure'].items():
    current = re.findall(r'\\(?:part|chapter|section)\*?\{[^\n]*', (root / name).read_text())
    assert current == expected, f'Changed structure: {name}'
for name in ['PastProblems.tex', 'NotationandConventions.tex',
             'MathematicsforScienceQuiz.tex', 'FactstoMemorize.tex',
             'Preface.tex', 'Acknowledgements.tex']:
    data = (root / name).read_bytes()
    blob = hashlib.sha1(b'blob ' + str(len(data)).encode() + b'\0' + data).hexdigest()
    assert blob == baseline['files'][name], f'Unexpected edit: {name}'
past = (root / 'PastProblems.tex').read_text()
assert len(re.findall(r'\\begin\{pastproblem\}', past)) == 103
assert len(re.findall(r'\\begin\{solution\}', past)) == 106
assert (root / 'MathematicsforScienceQuiz.pdf').read_bytes().startswith(b'%PDF-')
print(f'PASS: {len(manifest)} file hashes, unchanged structure, 103 problems / 106 solutions.')
