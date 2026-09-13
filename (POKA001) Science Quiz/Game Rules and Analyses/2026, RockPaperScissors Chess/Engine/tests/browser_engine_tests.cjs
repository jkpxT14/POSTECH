// RPSC 0.20.0 browser analyzer / Format 3 regression suite.
const fs=require('fs'),vm=require('vm'),assert=require('assert'),path=require('path');
const base=path.resolve(__dirname,'../..');
const html=fs.readFileSync(path.join(base,'RockPaperScissorsChess.html'),'utf8');
const parts=[...html.matchAll(/<script[^>]*>([\s\S]*?)<\/script>/g)].map(x=>x[1]);
assert.strictEqual(parts.length,2,'expected worker + UI scripts');
const ui=vm.createContext({console,performance,setTimeout,clearTimeout,module:{exports:{}},confirm:()=>true});
vm.runInContext(parts[1],ui);
const run=x=>vm.runInContext(x,ui);
assert.deepStrictEqual(JSON.parse(JSON.stringify(run('runSelfTests()'))),[]);
assert.strictEqual(run(`qText({POSTECH:1,KAIST:0})`),'Q[1, 0]');
assert.strictEqual(run(`ORI.length`),24);
assert.strictEqual(run(`generateRawMoves(emptyGame(),'W').length`),161);

// Canonical reverse assignment: Q[1,0] means POSTECH-only and therefore B gains the item.
ui.reverse=`[Format "3"]\n[White "Engine"]\n[Black "POSTECH Squad"]\n[WhiteTeam "KAIST"]\n[BlackTeam "POSTECH"]\n[Result "*"]\n[QOrder "POSTECH, KAIST"]\n\n1. Q[1, 0] B+St\n2. Q[0, 1] W+Ro`;
let rg=run('parseRecord(reverse)');ui.rg=rg;
assert.strictEqual(run('rg.q.POSTECH'),1);assert.strictEqual(run('rg.q.KAIST'),1);
assert.strictEqual(run('rg.items.B.St'),1);assert.strictEqual(run('rg.items.W.Ro'),1);
ui.bad=ui.reverse.replace('Q[1, 0] B+St','Q[1, 0] W+St');
assert.throws(()=>run('parseRecord(bad)'),/item must be B/);

// Legacy Format 2 interprets Q in White,Black order then migrates on serialization.
ui.legacy=`[Format "2"]\n[White "KAIST"]\n[Black "POSTECH"]\n[Result "*"]\n\n1. Q[0, 1] B+St`;
let lg=run('parseRecord(legacy)');ui.lg=lg;
assert.strictEqual(run('lg.q.POSTECH'),1);assert.strictEqual(run('lg.q.KAIST'),0);
assert.ok(run('serialize3(lg)').includes('1. Q[1, 0] B+St'));

// Format 3 round trip is semantic and textual canonicalization.
ui.canon=run('serialize3(rg)');ui.rg2=run('parseRecord(canon)');
assert.strictEqual(run('serialize3(rg2)'),ui.canon);

// Replay all six handbook games with explicit school-role mappings.
const games=fs.readFileSync(path.join(base,'Games.tex'),'utf8');
const blocks=[...games.matchAll(/\\begin\{gamerecord\}([\s\S]*?)\\end\{gamerecord\}/g)].map(x=>x[1]);
assert.strictEqual(blocks.length,6);
const headLines=games.split('\n').filter(l=>l.startsWith('\\gamehead'));
const maps=headLines.map(l=>{const m=l.match(/\}\{(POSTECH|KAIST)\}\{(POSTECH|KAIST)\}\{2026/);assert.ok(m,'school mapping in gamehead');return [m[1],m[2]];});
const expected=[[18,17,12,11,3,3],[15,23,11,11,2,6],[17,13,11,11,3,1],[20,18,14,12,3,3],[32,34,20,20,6,7],[25,23,11,11,7,6]];
for(let i=0;i<blocks.length;i++){
  const body=blocks[i].trim().split('\n').map(l=>l.trim().replace(/^([0-9]+\.)\s*&\s*/,'$1 ').replace(/\\\\\s*$/,'')).join('\n');
  const [wt,bt]=maps[i];
  ui.rec=`[Format "3"]\n[White "W"]\n[Black "B"]\n[WhiteTeam "${wt}"]\n[BlackTeam "${bt}"]\n[Result "*"]\n[QOrder "POSTECH, KAIST"]\n\n${body}`;
  let gg=run('parseRecord(rec)');ui.gg=gg;let s=run('scorePair(gg)');
  let got=[s.ws,s.bs,s.wq,s.bq,s.wc,s.bc];
  assert.deepStrictEqual(got,expected[i],`Game ${i+1}: ${got}`);
}

// Full legacy Game 6 fixture preserves the historical state when migrated.
ui.fullLegacy=fs.readFileSync(path.join(base,'Examples/Game6_Format2_legacy.rpsc'),'utf8');
let f2=run('parseRecord(fullLegacy)');ui.f2=f2;let s2=run('scorePair(f2)');
assert.deepStrictEqual([s2.ws,s2.bs,s2.wq,s2.bq,s2.wc,s2.bc],[25,23,11,11,7,6]);
let migrated=run('serialize3(f2)');
assert.ok(migrated.includes('[WhiteTeam "KAIST"]'));
assert.ok(migrated.includes('[BlackTeam "POSTECH"]'));
assert.ok(migrated.includes('5. Q[1, 0] B+St'));
assert.ok(migrated.includes('7. Q[0, 1] W+Pu'));


// Interactive state machine: first solo-correct may choose second; Q remains school-ordered.
run(`game=freshGame(); app.mode='hvh'; initHistory(); submitQuiz(1,0)`);
assert.strictEqual(run('game.phase'),'ORDER_CHOICE');
assert.strictEqual(run('game.orderChoiceSchool'),'POSTECH');
run(`chooseSolo(false)`); // POSTECH chooses second: WhiteTeam=KAIST, BlackTeam=POSTECH.
assert.strictEqual(run('game.teams.W'),'KAIST');assert.strictEqual(run('game.teams.B'),'POSTECH');
assert.strictEqual(run('game.phase'),'ITEM_SELECT');assert.strictEqual(run('game.itemRole'),'B');
run(`gainItem('St',false)`);assert.strictEqual(run('game.phase'),'QUIZ');
assert.ok(run(`serialize3(game)`).includes('1. Q[1, 0] B+St'));
run(`submitQuiz(1,1)`);assert.strictEqual(run('game.phase'),'MOVE');assert.strictEqual(run('game.moveRole'),'W');
run(`m1=generateRawMoves(game,'W')[0]; playMoveObject(m1,false)`);assert.strictEqual(run('game.moveRole'),'B');
run(`m2=generateRawMoves(game,'B')[0]; playMoveObject(m2,false)`);assert.strictEqual(run('game.phase'),'QUIZ');
assert.strictEqual(run('game.rounds[1].moves.length'),2);

// Worker and UI use the same exact orientation tables and reduced successor semantics.
const worker=vm.createContext({console,performance,postMessage:()=>{}});vm.runInContext(parts[0],worker);
ui.es=run(`engineState(emptyGame(),'W')`);worker.s=JSON.parse(JSON.stringify(ui.es));
assert.strictEqual(vm.runInContext('gen(s).length',worker),161);
assert.strictEqual(vm.runInContext('unique(s).length',worker),84);
let wr=vm.runInContext('analyze(s,150,0)',worker);
assert.ok(wr.candidates.length>=3,'worker should expose at least three candidates');
assert.strictEqual(new Set(wr.candidates.slice(0,3).map(x=>x.notation)).size,3);
console.log('Browser analyzer Format 3 / migration / six-game / worker regression suite passed.');
