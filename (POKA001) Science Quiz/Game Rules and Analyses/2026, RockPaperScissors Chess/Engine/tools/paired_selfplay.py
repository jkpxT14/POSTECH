#!/usr/bin/env python3
"""Paired RPSC engine match harness.

Usage:
  python paired_selfplay.py CANDIDATE BASELINE PAIRS START [MOVE_MS] [ITEM_MS]

Each scenario is played twice with the same quiz schedule and reversed engine colors.
"""
import subprocess, random, re, sys, json

class EngineProcess:
    def __init__(self, path):
        self.p = subprocess.Popen([path], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                  stderr=subprocess.PIPE, bufsize=0, text=False)
        self._until(b"rpsc\n", lambda x: x == b"rpscok\n")
    def _line(self):
        line = self.p.stdout.readline()
        if not line:
            raise RuntimeError("engine EOF: " + self.p.stderr.read().decode(errors="ignore"))
        return line
    def _until(self, data, stop):
        self.p.stdin.write(data); self.p.stdin.flush()
        while True:
            line = self._line()
            if stop(line): return
    def send(self, text):
        self.p.stdin.write((text + "\n").encode()); self.p.stdin.flush()
    def ok(self, text):
        self.send(text)
        got = self._line().decode().strip()
        if got != "ok": raise RuntimeError((text, got))
    def go(self, ms):
        self.send(f"go movetime {ms}")
        while True:
            line = self._line().decode().strip()
            if line.startswith("bestmove "):
                return line[len("bestmove "):]
    def choose_item(self, side, ms):
        self.send(f"chooseitem {side} movetime {ms}")
        while True:
            line = self._line().decode().strip()
            if line.startswith("bestitem "):
                return line.split()[1]
    def show(self):
        self.send("show"); return self._line().decode().strip()
    def close(self):
        try: self.send("quit")
        except Exception: pass
        try: self.p.wait(timeout=.2)
        except Exception: self.p.kill()

def schedule_for(index):
    rng = random.Random(20260910 + index)
    out = []
    for _ in range(20):
        x = rng.random()
        out.append("11" if x < .35 else "00" if x < .70 else "10" if x < .85 else "01")
    return out

def game(white_path, black_path, schedule, move_ms, item_ms):
    w, b = EngineProcess(white_path), EngineProcess(black_path)
    engines = (w, b); qw = qb = 0
    try:
        for rnd, q in enumerate(schedule, 1):
            qw += q[0] == "1"; qb += q[1] == "1"
            symmetric = q in ("11", "00")
            remaining = 2 * (21 - rnd) if symmetric else 2 * (20 - rnd)
            for e in engines: e.ok(f"match {int(qw)} {int(qb)} {remaining}")
            if q in ("10", "01"):
                side = "W" if q == "10" else "B"
                owner = w if side == "W" else b
                item = owner.choose_item(side, item_ms)
                for e in engines: e.ok(f"gain {side} {item}")
            else:
                move = w.go(move_ms)
                for e in engines: e.ok("move " + move)
                move = b.go(move_ms)
                for e in engines: e.ok("move " + move)
        sw = w.show(); sb = b.show()
        if sw != sb: raise RuntimeError(("desync", sw, sb))
        cw, cb = map(int, re.search(r"captures (\d+)-(\d+)", sw).groups())
        pw, pb = int(qw) + 2*cw, int(qb) + 2*cb
        winner = "W" if pw > pb or (pw == pb and qw > qb) else "B" if pb > pw or (pw == pb and qb > qw) else "D"
        return {"winner": winner, "score": [pw, pb], "quiz": [int(qw), int(qb)], "captures": [cw, cb]}
    finally:
        w.close(); b.close()

def main():
    if len(sys.argv) < 5:
        raise SystemExit(__doc__)
    cand, base, pairs, start = sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4])
    move_ms = int(sys.argv[5]) if len(sys.argv) > 5 else 50
    item_ms = int(sys.argv[6]) if len(sys.argv) > 6 else 20
    cp = 0.0
    for i in range(start, start+pairs):
        q = schedule_for(i)
        a = game(cand, base, q, move_ms, item_ms)
        b = game(base, cand, q, move_ms, item_ms)
        pa = 1 if a["winner"] == "W" else .5 if a["winner"] == "D" else 0
        pb = 1 if b["winner"] == "B" else .5 if b["winner"] == "D" else 0
        cp += pa + pb
        print(i, pa+pb, f"cumulative={cp/(2*(i-start+1)):.4f}", a, b, flush=True)
    print(json.dumps({"pairs": pairs, "games": 2*pairs, "candidate_points": cp,
                      "baseline_points": 2*pairs-cp, "score": cp/(2*pairs)}, indent=2))

if __name__ == "__main__": main()
