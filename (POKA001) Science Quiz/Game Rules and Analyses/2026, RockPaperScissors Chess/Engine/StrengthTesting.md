# Strength Testing

Baseline: RPSC Engine 0.6.0
Candidate: RPSC Engine 0.7.0
Mode: board-only `Only Q[0, 0]` proxy, 40 plies / 20 rounds
Pairing: every opening prefix is played twice with the candidate's color reversed
Opening generation: deterministic legal prefixes from two independent seeded sets

## 6,000-node exploratory match

48 distinct opening prefixes, 96 games total:

- Candidate wins: 47
- Draws: 12
- Candidate losses: 37
- Score: 55.21%
- Mean capture differential: +0.1875 capture/game
- Equivalent board-score differential: +0.375 point/game

The first 48-game opening set scored 26-6-16 (60.42%); the independent second set scored 21-6-21 (50.00%). The aggregate is encouraging but is not a statistically conclusive strength result.

## Higher-node confirmation sample

A separate 12,000-node sample over 12 paired opening prefixes (24 games) scored 4-11-9 (39.58%) for 0.7.0, with mean capture differential -0.3333. This smaller higher-budget sample does not confirm the lower-budget edge.

## Combined exploratory total

Across the two documented budgets there are 120 games: 51 wins, 23 draws, and 46 losses for 0.7.0, an overall score of 52.08% and mean capture differential of about +0.0833. Because the two budgets point in different directions, this combined number should not be treated as an Elo estimate.

## Interpretation

The current evidence is mixed. Version 0.7.0 has a positive aggregate in the broader 96-game exploratory set, while the smaller higher-node sample favors 0.6.0. Therefore the package documents the testing rather than claiming a measured Elo gain. The 0.7.0 changes are retained as a search/analysis development step because they are general, bounded, rule-derived, preserve correctness regression, and address tactical horizon behavior without position-specific evaluation terms.

Future engine versions should continue paired testing with more positions, longer matches, multiple budgets, and eventually a sequential statistical test once the match harness is mature.
