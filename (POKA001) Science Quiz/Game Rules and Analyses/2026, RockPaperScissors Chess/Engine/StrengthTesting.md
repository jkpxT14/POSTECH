# Strength Testing — Engine 0.20.0

A quiz schedule is always a sequence of `Q[POSTECH, KAIST]`. In each paired scenario the school-to-color mapping is fixed while the candidate and baseline engines swap White/Black. With the default harness mapping `WhiteTeam=POSTECH`, `BlackTeam=KAIST`, Game A assigns candidate to White/POSTECH and Game B assigns candidate to Black/KAIST. Therefore White move-order advantage and each school-only quiz opportunity are both exchanged between engines.

The harness uses `teams W POSTECH B KAIST` and sends quiz totals through `matchpk <POSTECHQuiz> <KAISTQuiz> <remainingPlies>`. It never treats `Q[1,0]` as a generic White opportunity.
