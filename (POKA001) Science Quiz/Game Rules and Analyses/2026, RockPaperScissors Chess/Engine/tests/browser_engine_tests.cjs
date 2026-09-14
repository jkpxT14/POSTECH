// RPSC 0.22.3 analyzer / Format 3 regression suite.
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

// Replay all seven handbook games with explicit school-role mappings.
const games=fs.readFileSync(path.join(base,'Games.tex'),'utf8');
const blocks=[...games.matchAll(/\\begin\{gamerecord\}([\s\S]*?)\\end\{gamerecord\}/g)].map(x=>x[1]);
assert.strictEqual(blocks.length,7);
const headLines=games.split('\n').filter(l=>l.startsWith('\\gamehead'));
const maps=headLines.map(l=>{const m=l.match(/\}\{(POSTECH|KAIST)\}\{(POSTECH|KAIST)\}\{2026/);assert.ok(m,'school mapping in gamehead');return [m[1],m[2]];});
const expected=[[18,17,12,11,3,3],[15,23,11,11,2,6],[17,13,11,11,3,1],[20,18,14,12,3,3],[32,34,20,20,6,7],[25,23,11,11,7,6],[20,18,10,10,5,4]];
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
assert.ok(run('goHistory.toString().includes("scheduleAutomation")'),'history navigation must restart context-appropriate live analysis');
assert.ok(run('goHistory.toString().includes("restoreLiveEvalFromCache")'),'history navigation must not display an eval inherited from another node');
assert.ok(run('goHistory.toString().includes("lastItemDecision=null")'),'history navigation must clear stale decision analysis from another node');
assert.ok(run('scheduleAutomation.toString().includes("isHistoricalView()")'),'loaded historical/variation positions must remain analysis-only and live-analyzed');
assert.ok(run(`scheduleAutomation.toString().includes('requestEngineItemChoice(\"item\")')`),'historical item-choice nodes must receive live engine analysis without auto-playing');
assert.ok(run(`scheduleAutomation.toString().includes('requestEngineInitialChoice(\"initial\")')`),'historical initial-decision nodes must receive live engine analysis without auto-playing');

// Position Eval remains live between board moves and uses White-role signs everywhere.
// A post-Black QUIZ node is analyzed as the next White move under equal-Q (Q[0,0] / Q[1,1]).
ui.document={getElementById:()=>({})};
run(`game=freshGame(); game.order={white:C.POSTECH,black:C.KAIST,source:'test'}; game.pieces=initialPieces(C.POSTECH,C.KAIST); game.rounds=[{n:1,q:{POSTECH:true,KAIST:true},itemGain:null,moves:[],complete:true}]; game.teams.POSTECH.quiz=1; game.teams.KAIST.quiz=1; game.phase='QUIZ'; game.moveRole=null; app.lastLiveEval=null; app.lastItemDecision=null; app.lastInitialDecision=null; app.engineThinking=false; clearEngineAnalysis()`);
assert.strictEqual(run('boardAnalysisPhase()'),true,'QUIZ after roles are assigned must be an analyzable board phase');
assert.strictEqual(run('analysisProbeState().side'),'W','equal-Q probe after Black must start with White');
assert.strictEqual(run('analysisProbeMoveNumber()'),2,'equal-Q probe must label the next board-move round');
const quizKey=run('positionFingerprint()');
assert.ok(quizKey,'QUIZ phase must have a live-analysis fingerprint');
const quizState=JSON.parse(JSON.stringify(run('analysisProbeState()')));
const quizWorker=vm.createContext({console,performance,postMessage:()=>{}});vm.runInContext(parts[0],quizWorker);quizWorker.s=quizState;
const quizStatic0=vm.runInContext('evalW(s)',quizWorker);
quizWorker.s2=JSON.parse(JSON.stringify(quizState));vm.runInContext('s2.q.W++; s2.q.B++',quizWorker);
const quizStatic1=vm.runInContext('evalW(s2)',quizWorker);
assert.strictEqual(quizStatic0,quizStatic1,'Q[0,0] and Q[1,1] must be evaluation-equivalent');
const quizResult=vm.runInContext('search(s,{depth:2,ms:350,multipv:3})',quizWorker);
assert.ok(quizResult.candidates.length>=3,'QUIZ assumption must produce Top 3 next-White candidates');
ui.quizResult=JSON.parse(JSON.stringify(quizResult));
run('app.engineAnalysis=quizResult; app.engineAnalysis.rootSide="W"; app.analysisKey=positionFingerprint(); app.analysisState=analysisProbeState(); app.analysisMoveNumber=analysisProbeMoveNumber()');
let quizEval=run('positionEvalHTML()');
ui.quizBest=quizResult.candidates[0].value;
assert.ok(quizEval.includes(run('evalText(quizBest)'))&&quizEval.includes('Q[0, 0] / Q[1, 1]'),'QUIZ phase must show a fresh equal-Q live evaluation');
let quizAnalysis=run('engineAnalysisHTML()');
assert.ok((quizAnalysis.match(/class="candidate/g)||[]).length>=3&&quizAnalysis.includes('PV&nbsp;&nbsp;'),'QUIZ phase must show Top 3 lines and PVs');
const key00=run('positionFingerprint()');
run('game.teams.POSTECH.quiz++; game.teams.KAIST.quiz++');
const key11=run('positionFingerprint()');
assert.strictEqual(key00,key11,'equal correct Quiz increments must share the same analysis cache key');
run('game.teams.POSTECH.quiz--; game.teams.KAIST.quiz--; game.phase="ITEM_SELECT"; game.itemTeam=C.KAIST; app.lastItemDecision={chooser:"B",team:C.KAIST,candidates:[{item:"Ro",value:65,pv:[]}],partial:false,totalMs:10000};');
let itemEval=run('positionEvalHTML()');
assert.ok(itemEval.includes('+0.65')&&itemEval.includes('Item Choice'),'item-choice live eval must remain White-role normalized even when Black chooses');
assert.ok(run('itemDecisionHTML()').includes('+0.65'),'item recommendation scores must use White-role signs');
run(`render=()=>{}; scheduleAutomation=()=>{}; game.rounds=[{n:1,q:{POSTECH:false,KAIST:true},itemGain:null,moves:[],complete:false}]; initHistory(); app.lastItemDecision={chooser:'B',team:C.KAIST,candidates:[{item:'Ro',value:65,pv:[]},{item:'Pu',value:25,pv:[]},{item:'St',value:-10,pv:[]}],bestItem:'St',partial:false,totalMs:10000}; game.teams.KAIST.items.Pu=0; selectItemGain('Pu')`);
assert.strictEqual(run('app.lastLiveEval.value'),25,'after a manual item choice, the retained eval must match the item actually selected');
run(`game=freshGame(); game.phase='ORDER_CHOICE'; game.orderChoiceTeam=C.POSTECH; app.lastInitialDecision={team:C.POSTECH,candidates:[{first:false,item:'Pu',value:70,whiteValue:-70,pv:[]}],partial:false,totalMs:10000}; app.engineThinking=false`);
let initialEval=run('positionEvalHTML()');
assert.ok(initialEval.includes('-0.70')&&initialEval.includes('Projected White')&&initialEval.includes('KAIST'),'initial decision eval must show the candidate White role, not chooser-sign evaluation');
run(`game=freshGame(); game.order={white:C.POSTECH,black:C.KAIST,source:'test'}; game.pieces=initialPieces(C.POSTECH,C.KAIST); game.teams.POSTECH.quiz=10; game.teams.KAIST.quiz=10; game.teams.POSTECH.captures=5; game.teams.KAIST.captures=4; game.phase='GAME_OVER'; game.result=C.POSTECH; app.lastLiveEval=null; clearEngineAnalysis()`);
let finalEval=run('positionEvalHTML()');
assert.ok(finalEval.includes('+2.00')&&finalEval.includes('Final board position'),'finished games must show exact White-role terminal evaluation instead of a blank');
run(`game.phase='OT_QUIZ'; game.teams.POSTECH.captures=4; game.teams.KAIST.captures=4; game.overtime=[]; app.lastLiveEval=null`);
let overtimeEval=run('positionEvalHTML()');
assert.ok(overtimeEval.includes('+0.00')&&overtimeEval.includes('Overtime'),'overtime board-frozen states must keep a numeric White-role evaluation');

// Full analysis-session round trip: main line + sideline + current variation node.
// Use a short legal prefix from Game 1, branch before White's first move, then save/load.
run(`render=()=>{}; scheduleAutomation=()=>{}; scheduleBackgroundAnalysis=()=>{}; stopEngineWorker=()=>{app.engineThinking=false}`);
ui.sessionRec=`[Format "3"]\n[White "W"]\n[Black "B"]\n[WhiteTeam "POSTECH"]\n[BlackTeam "KAIST"]\n[Result "*"]\n[QOrder "POSTECH, KAIST"]\n\n1. Q[1, 1] W1: a1-a2-a3-b3 B1: h8-h7-g7-f7\n2. Q[1, 0] W+St`;
run('sessionParsed=parseAndReplayDetailed(sessionRec); installLoadedHistory(sessionParsed)');
const postBlackQuiz=run('history.nodes.find(n=>n?.kind==="move"&&n.event?.role==="B"&&n.state?.phase==="QUIZ")');
assert.ok(postBlackQuiz,'replayed history must retain a node immediately after Black completes a move');
ui.postBlackId=postBlackQuiz.id;
run('game=clone(historyNode(postBlackId).state)');
assert.strictEqual(run('game.phase'),'QUIZ');
assert.ok(run('positionFingerprint()'),'post-Black historical QUIZ node must remain directly analyzable');
run('game=clone(historyNode(history.head).state)');
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

// Completed board analyses are persisted with an engine signature so loaded main/sideline nodes restore instantly.
run('cacheAnalysis(positionFingerprint(),sideResult,toEngineState(),analysisMoveNumber(game),game.moveRole,"background",0)');
assert.ok(run('app.analysisCache.size')>=1,'completed sideline analysis must enter the position cache');
assert.strictEqual(run('ANALYSIS_ENGINE_SIGNATURE'),'0.22.0-search');
const cachedKey=run('positionFingerprint()');ui.cachedKey=cachedKey;
const payload=run('sessionPayload()');ui.payload=JSON.parse(JSON.stringify(payload));
assert.strictEqual(payload.analysisEngine,'0.22.0-search');
assert.ok(Array.isArray(payload.analysisCache)&&payload.analysisCache.some(x=>x.key===cachedKey),'session payload must persist completed board analysis');
run('app.analysisCache.clear()');
assert.strictEqual(run('restoreAnalysisCacheFromSession(payload)'),payload.analysisCache.length,'compatible session analysis cache must restore');
assert.ok(run('showCachedAnalysis(cachedKey)'),'restored cache must immediately repopulate eval/Top 3/PV state');
assert.ok(run('engineAnalysisHTML()').includes('PV&nbsp;&nbsp;'),'restored cached analysis must retain PV lines');

// Session save must include the complete history tree and restore the active variation.
const savedEval=run('app.engineAnalysis.rootSide==="W"?app.engineAnalysis.candidates[0].value:-app.engineAnalysis.candidates[0].value');ui.savedEval=savedEval;
run(`app.lastLiveEval={value:37,whiteTeam:game.order.white,source:'board'}`);
const savedSession=run('sessionRecordText()');ui.savedSession=savedSession;
assert.ok(savedSession.includes('[Session "'),'analysis session payload must be embedded');
assert.ok(savedSession.includes('{Variation '),'human-readable variation summary must be present');
run('roundTrip=parseAndReplayDetailed(savedSession)');
assert.strictEqual(run('roundTrip.sessionFresh'),true,'saved Format 3 session must restore as fresh');
run('app.analysisCache.clear(); installLoadedHistory(roundTrip)');
assert.ok(run('app.analysisCache.size')>=1,'load must restore compatible saved board analyses');
assert.strictEqual(run('app.lastLiveEval.value'),savedEval,'node-specific cached evaluation must override a stale generic live-eval fallback');
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
console.log('RPSC 0.22.3 analyzer / Format 3 / persisted-analysis / continuous-live-eval / Top 3 PV regression suite passed.');
