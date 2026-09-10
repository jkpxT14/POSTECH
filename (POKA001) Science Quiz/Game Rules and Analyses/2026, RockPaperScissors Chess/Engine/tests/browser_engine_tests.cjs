// Run with Node.js; no browser dependency required for shared rules/search tests.
const fs=require('fs'),vm=require('vm'),assert=require('assert'),path=require('path');
const base=path.resolve(__dirname,'../..');
const html=fs.readFileSync(path.join(base,'RockPaperScissorsChess.html'),'utf8');
const parts=[...html.matchAll(/<script[^>]*>([\s\S]*?)<\/script>/g)].map(x=>x[1]);
const ui=vm.createContext({console,setTimeout,clearTimeout,performance});vm.runInContext(parts[1],ui);
const worker=vm.createContext({console,performance});vm.runInContext(parts[0],worker);
worker.s=vm.runInContext('initialOrderProbeState(C.POSTECH)',ui);
vm.runInContext(`
function assert(c,msg){if(!c)throw Error(msg)}
s.rem=-1;assert(generateMoves(s).length===161,'initial legal paths');
s.it.W={Pu:1,Ro:1,St:1};s.it.B={Pu:1,Ro:1,St:1};
assert(generateMoves(s).length===2146,'item-rich legal paths');
let raw=generateMoves(s);assert(raw.some(m=>m.item==='Pu'&&m.path[1][0]===m.path[0][0]&&m.path[1][1]===m.path[0][1]),'Push return');
for(let m of raw)for(let j=1;j<m.dirs.length;j++)assert(m.dirs[j]!==OP[m.dirs[j-1]],'Roll reversal');
function successor(m){let z=cloneState(s);doMove(z,m,0);return key(z)}
let exactSet=new Set(raw.map(successor)),reducedSet=new Set(uniqueMoves(s).map(x=>successor(x.m)));
assert(exactSet.size===568&&reducedSet.size===exactSet.size&&[...exactSet].every(x=>reducedSet.has(x)),'successor completeness');
function oracle(z){if(z.rem===0)return ev(z);let ms=uniqueMoves(z);if(!ms.length)return ev(z);let best=-1e9;for(let x of ms){let child=cloneState(z);doMove(child,x.m,0);best=Math.max(best,-oracle(child))}return best}
let cases=0;
for(let i=0;i<4;i++){
 s.rem=-1;let ms=uniqueMoves(s),m=ms[(i*137+61)%ms.length].m;doMove(s,m,0);s.rem=2;
 let scores=uniqueMoves(s).map(x=>{let z=cloneState(s);doMove(z,x.m,0);return -oracle(z)}).sort((a,b)=>b-a);
 let before=JSON.stringify(s),r=search(s,{depth:1,ms:60000,multipv:3});assert(r.depth===1,'completed depth');
 for(let k=0;k<3;k++){assert(r.candidates[k].value===scores[k],'exact endgame rank '+k);let z=cloneState(s);doMove(z,r.candidates[k].move,0);assert(-oracle(z)===r.candidates[k].value,'candidate score');cases++}
 assert(JSON.stringify(s)===before,'search mutates root');
 let resumed=search(s,{depth:64,ms:0.001,multipv:3,startDepth:2,seedResult:r});assert(resumed.depth>=r.depth,'completed cache lost');
 for(let k=0;k<3;k++)assert(resumed.candidates[k].value===r.candidates[k].value,'interrupted MultiPV changed');
}
s.rem=0;assert(search(s,{depth:1,ms:10,multipv:3}).best===null,'terminal move');
console.log('Browser engine: legal-path/successor checks and '+cases+' exact endgame rank checks passed');
`,worker);
// Replay all textbook game records with the UI's actual notation parser.
const games=fs.readFileSync(path.join(base,'Games.tex'),'utf8');let counts=[];
for(const block of games.matchAll(/\\begin\{gamerecord\}([\s\S]*?)\\end\{gamerecord\}/g)){
 const rows=block[1].trim().split('\n').map(l=>l.trim().replace(/\s*&\s*/,' ').replace(/\\\\\s*$/,''));
 ui.record='[Event "Regression"]\n[White "POSTECH"]\n[Black "KAIST"]\n[Result "*"]\n\n'+rows.join('\n');
 counts.push(vm.runInContext('(()=>{let g=parseAndReplayDetailed(record).game;return [g.rounds.length,g.teams.POSTECH.captures,g.teams.KAIST.captures]})()',ui));
}
assert.deepStrictEqual(JSON.parse(JSON.stringify(counts)),[[20,3,3],[20,2,6],[20,3,1],[20,3,3],[20,6,7],[20,7,6]]);
console.log('Handbook: six games / 120 quiz rounds replayed; captures match');
