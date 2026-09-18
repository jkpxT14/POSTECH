const fs = require('fs');
const vm = require('vm');
const assert = require('assert');
const path = require('path');

const base = path.resolve(__dirname, '../..');
const html = fs.readFileSync(path.join(base, 'RockPaperScissorsChess.html'), 'utf8');
assert.ok(html.includes('<meta name="rpsc-version" content="0.25.0">'));
assert.ok(html.includes('ANALYSIS_ENGINE_SIGNATURE="0.25.0-swe-transition4-pb3-resign"'));
assert.ok(html.includes('<meta name="rpsc-edition" content="2026 Science War Edition">'));
assert.ok(html.includes('<meta name="rpsc-edition-date" content="2026-09-17">'));
assert.ok(html.includes('ms:10000,multipv:3'), 'default analysis keeps Top 3 for 10 s');
assert.ok(html.includes('20000-seedElapsed'), 'Analyze continues cached search toward cumulative 20 s');

const scripts = [...html.matchAll(/<script[^>]*>([\s\S]*?)<\/script>/g)].map(x => x[1]);
assert.strictEqual(scripts.length, 2, 'expected worker + UI scripts');

const ui = vm.createContext({
  console, performance, setTimeout, clearTimeout,
  module: {exports: {}}, confirm: () => true,
  btoa, atob, escape, unescape, encodeURIComponent, decodeURIComponent
});
vm.runInContext(scripts[1], ui);
const run = expr => vm.runInContext(expr, ui);

assert.strictEqual(run('ALL_ORIENTATIONS.length'), 24);
assert.strictEqual(run(`qNotation({POSTECH:true,KAIST:false})`), 'Q[1, 0]');
assert.strictEqual(run(`qNotation({POSTECH:false,KAIST:true})`), 'Q[0, 1]');
assert.ok(run('requestEngineSearch.toString().includes("multipv:3")'));
assert.ok(run('requestEngineSearch.toString().includes("20000-seedElapsed")'));

ui.reverse = `[Format "3"]\n[White "Engine"]\n[Black "Squad"]\n[WhiteTeam "KAIST"]\n[BlackTeam "POSTECH"]\n[Result "*"]\n[QOrder "POSTECH, KAIST"]\n\n1. Q[1, 0] B+St\n2. Q[0, 1] W+Ro`;
run('rg=parseAndReplayDetailed(reverse).game');
assert.strictEqual(run('rg.teams.POSTECH.quiz'), 1);
assert.strictEqual(run('rg.teams.KAIST.quiz'), 1);
assert.strictEqual(run('rg.teams.POSTECH.items.St'), 1);
assert.strictEqual(run('rg.teams.KAIST.items.Ro'), 1);
assert.ok(run('recordText(rg)').includes('[QOrder "POSTECH, KAIST"]'));

ui.resignation = `[Format "3"]
[Event "Resignation Test"]
[Date "2026.09.17"]
[White "Engine"]
[Black "Squad"]
[WhiteTeam "KAIST"]
[BlackTeam "POSTECH"]
[Result "1-0"]
[Termination "Resignation"]
[Score "1-0"]
[Quiz "1-0"]
[Captures "0-0"]
[QOrder "POSTECH, KAIST"]

1. Q[0, 1] W+Pu`;
run('rr=parseAndReplayDetailed(resignation).game');
assert.strictEqual(run('rr.result'), 'KAIST');
assert.strictEqual(run('rr.termination.type'), 'Resignation');
assert.strictEqual(run('rr.termination.resignedTeam'), 'POSTECH');
assert.ok(run('recordText(rr)').includes('[Termination "Resignation"]'));
assert.ok(html.includes('id="resignBtn"'), 'Analysis Board must expose a manual Resign control');

// Build a normal analyzable position with the UI's exact state encoder.
run(`game=freshGame(); game.order={white:C.POSTECH,black:C.KAIST,source:'test'}; game.pieces=initialPieces(C.POSTECH,C.KAIST); game.phase='MOVE'; game.moveRole='W'; game.rounds=[{n:1,q:{POSTECH:true,KAIST:true},itemGain:null,moves:[],complete:false}]`);
ui.state = JSON.parse(JSON.stringify(run('toEngineState()')));
const worker = vm.createContext({console, performance, postMessage: () => {}});
vm.runInContext(scripts[0], worker);
worker.state = ui.state;
const result = vm.runInContext('search(state,{depth:2,ms:700,multipv:3})', worker);
assert.ok(result.candidates.length >= 3, 'engine must produce Top 3');
const sig = m => JSON.stringify(m);
assert.strictEqual(new Set(result.candidates.slice(0,3).map(c => sig(c.move))).size, 3, 'Top 3 must be distinct');

// Embedded worker must remain the exact strength-tested reference core modulo HTML newlines.
const ref = fs.readFileSync(path.join(base, 'Engine/rpsc_engine_0.25.0.js'), 'utf8').replace(/^\n+|\n+$/g, '');
const embedded = scripts[0].replace(/^\n+|\n+$/g, '');
assert.ok(!/resign/i.test(embedded), 'embedded search worker must never auto-resign');
assert.ok(embedded.includes('if(transition)extra=Math.max(extra,4)'), 'Science War engine must retain the selected low-material transition extension');
assert.strictEqual(embedded, ref, 'embedded worker diverged from production Science War engine');

console.log('Browser/engine contract tests passed.');
