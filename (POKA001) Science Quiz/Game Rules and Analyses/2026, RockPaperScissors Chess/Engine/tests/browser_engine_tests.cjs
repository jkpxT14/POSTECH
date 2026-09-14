// RPSC 0.22.0 analyzer / Format 3 regression suite.
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
assert.ok(!html.includes('saveBtn").disabled=isHistoricalView()'),'Save remains available on a complete sideline position');

const ui=vm.createContext({console,performance,setTimeout,clearTimeout,module:{exports:{}},confirm:()=>true,btoa,atob,escape,unescape,encodeURIComponent,decodeURIComponent});
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
assert.ok(run('goHistory.toString().includes("scheduleBackgroundAnalysis")'),'history navigation must restart live background analysis');
assert.ok(run('scheduleAutomation.toString().includes("isHistoricalView()")'),'loaded historical/variation positions must remain analysis-only and live-analyzed');

// Full analysis-session round trip: main line + sideline + current variation node.
// Use a short legal prefix from Game 1, branch before White's first move, then save/load.
run(`render=()=>{}; scheduleAutomation=()=>{}; scheduleBackgroundAnalysis=()=>{}; stopEngineWorker=()=>{app.engineThinking=false}`);
ui.sessionRec=`[Format "3"]\n[White "W"]\n[Black "B"]\n[WhiteTeam "POSTECH"]\n[BlackTeam "KAIST"]\n[Result "*"]\n[QOrder "POSTECH, KAIST"]\n\n1. Q[1, 1] W1: a1-a2-a3-b3 B1: h8-h7-g7-f7\n2. Q[1, 0] W+St`;
run('sessionParsed=parseAndReplayDetailed(sessionRec); installLoadedHistory(sessionParsed)');
const canonicalHead=run('history.head');
const branchParent=run('history.nodes.find(n=>n.state.phase==="MOVE"&&n.state.moveRole==="W").id');
ui.branchParent=branchParent;
run('history.current=branchParent; game=clone(historyNode(branchParent).state); app.variationActive=false');
assert.strictEqual(run('historyPlayedMove()'),'W1: a1-a2-a3-b3');

// Build one legal alternate from the embedded engine and commit it as a sideline.
const branchWorker=vm.createContext({console,performance,postMessage:()=>{}});vm.runInContext(parts[0],branchWorker);
branchWorker.s=JSON.parse(JSON.stringify(run('toEngineState()')));
const branchMoves=vm.runInContext('uniqueMoves(s).map(x=>x.m)',branchWorker);
ui.branchMoves=branchMoves;
const altIndex=run('branchMoves.findIndex(m=>engineMoveText(toEngineState(),m).notation!==historyPlayedMove())');
assert.ok(altIndex>=0,'an alternate legal move must exist');
ui.altMove=branchMoves[altIndex];
run('full=draftFromEngineMove(altMove); ui.draft=full; applyConfirmedDraft()');
assert.strictEqual(run('history.head'),canonicalHead,'canonical main-line head must remain unchanged');
assert.strictEqual(run('historyNode().onMain'),false,'alternate move must create a variation node');
assert.strictEqual(run('app.variationActive'),true);
const variationId=run('history.current');ui.variationId=variationId;

// Live evaluation and Top 3 must work on the sideline position too.
branchWorker.s=JSON.parse(JSON.stringify(run('toEngineState()')));
const sideResult=vm.runInContext('search(s,{depth:2,ms:350,multipv:3})',branchWorker);
assert.ok(sideResult.candidates.length>=3,'sideline search must return Top 3');
ui.sideResult=JSON.parse(JSON.stringify(sideResult));
run('app.engineAnalysis=sideResult; app.engineAnalysis.rootSide=game.moveRole; app.analysisKey=positionFingerprint(); app.analysisState=toEngineState(); app.analysisMoveNumber=analysisMoveNumber(game)');
ui.document={getElementById:()=>({})};
const sideEval=run('positionEvalHTML()');
assert.ok(sideEval.includes('Position Eval')&&sideEval.includes('White'),'sideline evaluation must be White-role normalized');
const sideAnalysis=run('engineAnalysisHTML()');
assert.ok((sideAnalysis.match(/class="candidate/g)||[]).length>=3,'sideline analysis must render Top 3 candidates');
assert.ok((sideAnalysis.match(/PV&nbsp;&nbsp;/g)||[]).length>=3,'sideline candidates must render PV lines');

// Session save must include the complete history tree and restore the active variation.
const savedSession=run('sessionRecordText()');ui.savedSession=savedSession;
assert.ok(savedSession.includes('[Session "'),'analysis session payload must be embedded');
assert.ok(savedSession.includes('{Variation '),'human-readable variation summary must be present');
run('roundTrip=parseAndReplayDetailed(savedSession)');
assert.strictEqual(run('roundTrip.sessionFresh'),true,'saved Format 3 session must restore as fresh');
run('installLoadedHistory(roundTrip)');
assert.strictEqual(run('history.current'),variationId,'load must restore the active sideline node');
assert.strictEqual(run('history.head'),canonicalHead,'load must preserve canonical main-line head');
assert.strictEqual(run('historyNode().onMain'),false);
assert.strictEqual(run('app.variationActive'),true);

// After loading, navigate away and back through the variation, then return to the main line.
const beforePrev=run('history.current');
run('historyPrevious()');
assert.notStrictEqual(run('history.current'),beforePrev,'Previous must leave the variation leaf');
run('goHistory(variationId)');
assert.strictEqual(run('history.current'),beforePrev,'saved variation must remain directly navigable after moving away');
run('returnToCurrent()');
assert.strictEqual(run('history.current'),run('history.head'),'Current must return to canonical main line');

// Worker keeps the mature MultiPV interface and the strengthened selective search.
const worker=vm.createContext({console,performance,postMessage:()=>{}});vm.runInContext(parts[0],worker);
assert.strictEqual(vm.runInContext('typeof tacticalReach',worker),'function');
assert.ok(parts[0].includes('extUsed<1'),'browser worker must use the one-step selective-extension limit');
assert.ok(parts[0].includes('x.t>=0||x.reset'),'browser quiescence must skip non-reset losing sacrifices');
ui.es=run(`toEngineState('W')`);worker.s=JSON.parse(JSON.stringify(ui.es));
let legal=vm.runInContext('uniqueMoves(s).length',worker);
assert.ok(legal>3,'worker must have multiple legal successors');
let result=vm.runInContext('search(s,{depth:3,ms:500,multipv:3})',worker);
assert.ok(result.candidates.length>=3,'worker must expose Top 3 candidates');
assert.strictEqual(new Set(result.candidates.slice(0,3).map(x=>JSON.stringify(x.move))).size,3);
assert.ok(result.candidates.slice(0,3).every(x=>Array.isArray(x.pv)&&x.pv.length>=1),'each candidate must carry a PV');
console.log('RPSC 0.22.0 analyzer / Format 3 / session / live-eval / Top 3 PV regression suite passed.');
