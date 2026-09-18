# Engine reference / regression material

- `rpsc_engine_0.24.1.js`: previous production champion used as the direct 0.25 strength baseline.
- `baseline_0.23.0.js`: earlier baseline retained for historical paired regression.
- `arena.js`: local paired-board benchmark harness; by default it loads the final `../rpsc_engine_0.25.0.js` as candidate.
- `results/paired_regression_56.jsonl` and `results/SUMMARY.txt`: historical 0.24-vs-0.23 regression records retained for reproducibility.

The production Science War engine is `../rpsc_engine_0.25.0.js`. The Analysis Board embeds that file exactly.
