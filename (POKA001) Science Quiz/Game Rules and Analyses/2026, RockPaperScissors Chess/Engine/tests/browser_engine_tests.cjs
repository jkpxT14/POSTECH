// RPSC 0.21.1 recovered analyzer / Format 3 regression suite.
const fs=require('fs'),vm=require('vm'),assert=require('assert'),path=require('path');
const base=path.resolve(__dirname,'../..');
const html=fs.readFileSync(path.join(base,'RockPaperScissorsChess.html'),'utf8');
const parts=[...html.matchAll(/<script[^>]*>([\s\S]*?)<\/script>/g)].map(x=>x[1]);
assert.strictEqual(parts.length,2,'expected worker + UI scripts');
assert.ok(html.includes('Jungwoo Kim (<span class="postech">POSTECH</span>)'),'byline restored');
assert.ok(html.includes('<svg id="board"'),'SVG board restored');
assert.ok(!html.includes('Deep Analyze'),'no separate Deep Analyze control');
assert.ok(!html.includes('class="coord file"'),'no per-square coordinate clutter');
assert.ok(html.includes('Position Eval'),'current evaluation surface present');
assert.ok(html.includes('ms:10000,multipv:3'),'normal board analysis remains 10 seconds');
assert.ok(html.includes('20000-seedElapsed'),'Analyze extends the same search toward 20 seconds');

const ui=vm.createContext({console,performance,setTimeout,clearTimeout,module:{exports:{}},confirm:()=>true});
vm.runInContext(parts[1],ui);
const run=x=>vm.runInContext(x,ui);
assert.strictEqual(run('ALL_ORIENTATIONS.length'),24);
assert.strictEqual(run(`qNotation({POSTECH:true,KAIST:false})`),'Q[1, 0]');
assert.strictEqual((run('boardSVG()').match(/data-piece=/g)||[]).length,8,'opening preview must show eight pieces');
assert.ok(run('boardSVG()').includes('>a<') || run('boardSVG()').includes('>1<'),'outer coordinates present');

ui.reverse=`[Format "3"]\n[White "Engine"]\n[Black "Squad"]\n[WhiteTeam "KAIST"]\n[BlackTeam "POSTECH"]\n[Result "*"]\n[QOrder "POSTECH, KAIST"]\n\n1. Q[1, 0] B+St\n2. Q[0, 1] W+Ro`;
let rg=run('parseAndReplayDetailed(reverse).game');ui.rg=rg;
assert.strictEqual(run('rg.teams.POSTECH.quiz'),1);
assert.strictEqual(run('rg.teams.KAIST.quiz'),1);
assert.strictEqual(run('rg.teams.POSTECH.items.St'),1);
assert.strictEqual(run('rg.teams.KAIST.items.Ro'),1);
assert.ok(run('recordText(rg)').includes('[WhiteTeam "KAIST"]'));
assert.ok(run('recordText(rg)').includes('[BlackTeam "POSTECH"]'));
assert.ok(run('recordText(rg)').includes('[QOrder "POSTECH, KAIST"]'));
assert.ok(run('recordText(rg)').includes('1. Q[1, 0] B+St'));

ui.legacy=`[Format "2"]\n[White "KAIST"]\n[Black "POSTECH"]\n[Result "*"]\n\n1. Q[0, 1] B+St`;
let lg=run('parseAndReplayDetailed(legacy).game');ui.lg=lg;
assert.strictEqual(run('lg.teams.POSTECH.quiz'),1);
assert.strictEqual(run('lg.teams.KAIST.quiz'),0);
assert.ok(run('recordText(lg)').includes('1. Q[1, 0] B+St'));

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
  let gg=run('parseAndReplayDetailed(rec).game');ui.gg=gg;
  let got=run(`[score(gg.order.white,gg),score(gg.order.black,gg),gg.teams[gg.order.white].quiz,gg.teams[gg.order.black].quiz,gg.teams[gg.order.white].captures,gg.teams[gg.order.black].captures]`);
  assert.deepStrictEqual(JSON.parse(JSON.stringify(got)),expected[i],`Game ${i+1}: ${got}`);
}

// History remains a tree with sideline support.
run(`game=freshGame(); game.order={white:C.POSTECH,black:C.KAIST,source:'test'}; game.pieces=initialPieces(C.POSTECH,C.KAIST); initHistory()`);
assert.strictEqual(run('Array.isArray(history.nodes[0].children)'),true);
assert.strictEqual(run('typeof historyPrevious'), 'function');
assert.strictEqual(run('typeof returnToCurrent'), 'function');
assert.strictEqual(run('typeof renderVariation'), 'function');

// Worker keeps the mature MultiPV interface and the strengthened selective search.
const worker=vm.createContext({console,performance,postMessage:()=>{}});vm.runInContext(parts[0],worker);
assert.strictEqual(vm.runInContext('typeof tacticalReach',worker),'function');
ui.es=run(`toEngineState('W')`);worker.s=JSON.parse(JSON.stringify(ui.es));
let legal=vm.runInContext('uniqueMoves(s).length',worker);
assert.ok(legal>3,'worker must have multiple legal successors');
let result=vm.runInContext('search(s,{depth:3,ms:500,multipv:3})',worker);
assert.ok(result.candidates.length>=3,'worker must expose Top 3 candidates');
assert.strictEqual(new Set(result.candidates.slice(0,3).map(x=>JSON.stringify(x.move))).size,3);
assert.ok(result.candidates.slice(0,3).every(x=>Array.isArray(x.pv)&&x.pv.length>=1),'each candidate must carry a PV');
console.log('Recovered analyzer / Format 3 / Top 3 PV regression suite passed.');
