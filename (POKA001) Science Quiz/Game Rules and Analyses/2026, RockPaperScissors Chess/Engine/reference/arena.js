const fs=require('fs'),vm=require('vm');
const {performance}=require('perf_hooks');
const path=require('path');
const HERE=__dirname;
const baselineSrc=fs.readFileSync(path.join(HERE,'baseline_0.23.0.js'),'utf8');
function makeEngine(src,label){
  const ctx={performance,console,postMessage:()=>{},onmessage:null};
  vm.createContext(ctx);
  vm.runInContext(src+`\n;globalThis.__api={search,chooseItem,chooseInitial,cloneState,doMoveWithUndo,uniqueMoves,evalW,key};`,ctx,{timeout:10000});
  return {label,api:ctx.__api};
}
function candidateSource(){return fs.readFileSync(path.join(HERE,'rpsc_engine_0.24.0.js'),'utf8');}

// orientation/rules constants, copied from app's exact definitions
const DIR_ORDER=['N','S','E','W'];
const VEC_TO_DIR=new Map([['0,1,0','N'],['0,-1,0','S'],['1,0,0','E'],['-1,0,0','W'],['0,0,1','UP'],['0,0,-1','DOWN']]);
const BASE_CUBE=[{id:'S1',gesture:'S',normal:[0,0,1],wrist:[0,-1,0]},{id:'S2',gesture:'S',normal:[0,0,-1],wrist:[0,-1,0]},{id:'R1',gesture:'R',normal:[0,1,0],wrist:[0,0,1]},{id:'R2',gesture:'R',normal:[0,-1,0],wrist:[0,0,-1]},{id:'P1',gesture:'P',normal:[-1,0,0],wrist:[0,-1,0]},{id:'P2',gesture:'P',normal:[1,0,0],wrist:[0,-1,0]}];
function clone(x){return JSON.parse(JSON.stringify(x))}
function sameVec(a,b){return a[0]===b[0]&&a[1]===b[1]&&a[2]===b[2]}
function vecKey(v){return v.join(',')}
function topFace(o){return o.find(q=>sameVec(q.normal,[0,0,1]))}
function topGesture(o){return topFace(o).gesture}
function transformVec(v,op){let[x,y,z]=v;switch(op){case'N':return[x,-z,y];case'S':return[x,z,-y];case'E':return[z,y,-x];case'W':return[-z,y,x];case'CW':return[y,-x,z];case'CCW':return[-y,x,z]}}
function cubeTransform(o,op){return o.map(f=>({id:f.id,gesture:f.gesture,normal:transformVec(f.normal,op),wrist:transformVec(f.wrist,op)}))}
function orientationKey(o){return o.slice().sort((a,b)=>a.id.localeCompare(b.id)).map(f=>`${f.id}:${f.normal.join(',')}/${f.wrist.join(',')}`).join('|')}
function allOrientations(){let first=clone(BASE_CUBE),mm=new Map([[orientationKey(first),first]]),q=[first];while(q.length){let s=q.shift();for(let d of DIR_ORDER){let n=cubeTransform(s,d),k=orientationKey(n);if(!mm.has(k)){mm.set(k,n);q.push(n)}}}return [...mm.values()]}
const ALL_ORIENTATIONS=allOrientations(),ORI=new Map(ALL_ORIENTATIONS.map((o,i)=>[orientationKey(o),i]));
function topWrist(o){return topFace(o).wrist}
function canonicalOrientation(g,w){let preferred=g+'1',matches=ALL_ORIENTATIONS.filter(o=>topGesture(o)===g&&VEC_TO_DIR.get(vecKey(topWrist(o)))===w);return clone(matches.find(o=>topFace(o).id===preferred)||matches[0])}
const ROLL=ALL_ORIENTATIONS.map(o=>DIR_ORDER.map(d=>ORI.get(orientationKey(cubeTransform(o,d))))),ROTL=ALL_ORIENTATIONS.map(o=>ORI.get(orientationKey(cubeTransform(o,'CCW')))),ROTR=ALL_ORIENTATIONS.map(o=>ORI.get(orientationKey(cubeTransform(o,'CW')))),TOP=ALL_ORIENTATIONS.map(topGesture),RED=ALL_ORIENTATIONS.map(o=>{let g=a=>{let f=o.find(q=>sameVec(q.normal,a));return f.gesture};return g([0,0,1])+g([0,1,0])+g([1,0,0])}),CANON={};for(let g of ['S','R','P'])for(let w of ['N','S'])CANON[g+w]=ORI.get(orientationKey(canonicalOrientation(g,w)));
function initialPieces(){let starts=[[0,0,'S'],[2,0,'R'],[4,0,'P'],[6,0,'S'],[7,7,'S'],[5,7,'R'],[3,7,'P'],[1,7,'S']];return starts.map((z,i)=>({id:(i<4?'W':'B')+(i%4+1),a:1,x:z[0],y:z[1],o:CANON[z[2]+(i<4?'S':'N')]}))}
function makeState(){return{side:'W',c:{W:0,B:0},q:{W:0,B:0},rem:40,it:{W:{Pu:0,Ro:0,St:0},B:{Pu:0,Ro:0,St:0}},p:initialPieces(),roll:ROLL,rotL:ROTL,rotR:ROTR,top:TOP,red:RED,canon:CANON}}
function rng(seed){let x=seed>>>0;return()=>{x=(x*1664525+1013904223)>>>0;return x/4294967296}}
function remainingPlies(round,side){return Math.max(0,(side==='W'?2:1)+2*(20-round));}
function resetSearchState(s,round,side){s.side=side;s.rem=remainingPlies(round,side)}
function applyActual(api,s,m){let u=new Int16Array(44);api.doMoveWithUndo(s,m,u);}
function scoreRole(s,role){return s.q[role]+2*s.c[role]}
function engineForRole(assign,role){return role==='W'?assign.W:assign.B}
function playGame(engineA,engineB,seed,moveMs=20,itemMs=25,initialMs=40,swap=false){
  const R=rng(seed), s=makeState();
  // Benchmark isolates board-engine strength. Quiz stream, role assignment and item gains are paired external inputs.
  const aWhite=(R()<0.5)!==swap, roleOf={A:aWhite?'W':'B',B:aWhite?'B':'W'}, engTeam={A:engineA,B:engineB};
  for(let round=1;round<=20;round++){
    const qa=R()<0.5,qb=R()<0.5;
    s.q[roleOf.A]+=qa?1:0;s.q[roleOf.B]+=qb?1:0;
    if(qa!==qb){
      const winner=qa?'A':'B',role=roleOf[winner],names=['Pu','Ro','St'];
      // Same seeded external item distribution for paired fairness.
      s.it[role][names[Math.floor(R()*3)]]++;
      continue;
    }
    for(let role of ['W','B']){
      const team=roleOf.A===role?'A':'B',eng=engTeam[team];
      resetSearchState(s,round,role);
      const r=eng.api.search(eng.api.cloneState(s),{ms:moveMs,depth:64,multipv:3},null);
      if(r.best)applyActual(eng.api,s,r.best);
    }
  }
  const ar=roleOf.A,br=roleOf.B,sa=scoreRole(s,ar),sb=scoreRole(s,br);
  if(sa!==sb)return sa>sb?1:-1;
  if(s.q[ar]!==s.q[br])return s.q[ar]>s.q[br]?1:-1;
  return 0;
}
if(require.main===module){
 const base=makeEngine(baselineSrc,'base'),cand=makeEngine(candidateSource(baselineSrc),'cand');
 let W=0,L=0,D=0,N=+(process.argv[2]||20),ms=+(process.argv[3]||10);let t=performance.now();
 for(let i=0;i<N;i++){let pair=Math.floor(i/2),rev=i%2===1,r=rev?-playGame(base,cand,12345+pair,ms,Math.max(10,ms),Math.max(15,ms*2),false):playGame(cand,base,12345+pair,ms,Math.max(10,ms),Math.max(15,ms*2),false);if(r>0)W++;else if(r<0)L++;else D++;if((i+1)%10===0)console.log(i+1,W,L,D)}
 console.log({N,W,L,D,rate:W/N,score:(W+.5*D)/N,ms,seconds:(performance.now()-t)/1000});
}
module.exports={baselineSrc,makeEngine,makeState,playGame};
