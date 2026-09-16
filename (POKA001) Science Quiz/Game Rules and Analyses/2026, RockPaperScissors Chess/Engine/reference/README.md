# Engine reference / regression material

- `rpsc_engine_0.24.0.js`: exact 0.24.0 worker core embedded in the Analysis Board.
- `baseline_0.23.0.js`: previous baseline used for paired strength comparison.
- `arena.js`: local paired-board benchmark harness.
- `results/paired_regression_56.jsonl`: exact 56-game regression records.
- `results/SUMMARY.txt`: aggregate result.

These files exist to make the 55%+ engine-strength gate auditable; the production Analysis Board is the HTML file at the project root, `../rpsc_engine_0.24.1.js` is the directly reusable production search core, and the native engine source is in `../src/`.
