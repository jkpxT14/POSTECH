# v2.3 - Final typography and handbook sync

Edition date: 2026-09-17

## 방향

- GitHub `main`의 44쪽 v2.2를 정보량의 기준으로 삼고, 이전 29쪽 재작성본에서 실제로 개선된 요소만 흡수했다.
- 페이지 수를 줄이는 것을 목표로 하지 않았다. 필요한 개념·공식·정리·암기 요소를 보존한 뒤 중복과 장황한 설명을 정리했고, 최종 결과는 43쪽이다.
- 목표는 경기 전날 또는 경기 직전 2–3시간 내 1회독 가능한 고밀도 Final이다.

## 복원·보존

29쪽 버전에서 지나치게 줄어들었던 내용을 복원했다. 특히 다음 영역의 고효율 toolkit을 보존했다.

- 미적분/해석: 부분적분·치환, 급수, Wallis, Sophomore's Dream, multivariable chain rule, Lagrange multipliers, change of variables, Cantor set, Cauchy–Riemann 등
- 선형대수: determinant toolkit, invertibility equivalents, characteristic polynomial, Cayley–Hamilton, special matrices, outer products 등
- 조합/이산: lattice paths, derangements, Vandermonde, Hall, Ramsey, graph complement, Master theorem, Eulerian/planar graph, Moser 등
- 확률: independence, total expectation/variance, covariance, memorylessness, tail-sum, standard distributions, stopping-time anchors 등
- 기하/정수론/대수/위상: 기존 v2.2의 재사용성 높은 핵심 결과를 폭넓게 보존
- 암기: Fields/Abel chronology, 수학자 연대기, 저작, 역사, 일화, 기관, famous problems를 충분히 보존

## 세 번의 전수 audit에서 반영한 보강

- finite-field multiplicative power map criterion
- coefficient extraction + Fibonacci generating function
- Conjugate Root Theorem
- Shou-Wu Zhang ↔ Andrew Wiles Medal
- John Charles Fields = Canadian mathematician
- Cavalieri's principle
- Kummer carry interpretation
- finitely generated group countability anchor
- Möbius / fractional-linear recurrence recognition cue

잘못되었거나 불안정한 source item은 넣지 않았다. 예: Collatz `77671`, 모순인 functional equation, 불안정한 topology 보기, 과도하게 특수한 exact group enumeration.

## 디자인·표기

- 본교재의 Chapter / Section 계층을 문서형 Final에 맞게 적용했다.
- main theorem-like environments는 `Theorem. Title` 뒤 줄바꿈 후 본문이 시작되도록 통일했다.
- heading과 본문 사이를 가운데점으로 잇는 디자인을 제거했다.
- 잘못된 U+318D `ㆍ`는 제거하고, 표준 가운데점 `·`는 실제 병렬 나열에만 제한적으로 사용했다.
- MATH000 convention을 반영: `int A` / `A^\circ`, closure `\overline A`, principal `Arg z\in(-\pi,\pi]`, `log z` / `Log z`, geometric distribution은 첫 성공 시행까지 센다.
- 암기 파트는 한국어 문서성을 강화하고, 주요 수학자 이름은 한국어 표기와 원어 식별을 함께 사용할 수 있게 정리했다.

## QA

- XeLaTeX clean build
- LaTeX warning / overfull / underfull / missing-character 점검
- 43쪽 전체 PDF 렌더링 후 시각 검토
- PDF preflight 통과


## v2.3 typography / design pass

- 43쪽 hybrid Final의 본문 내용은 유지하고 typography와 문서 디자인을 본교재에 다시 맞췄다.
- 영문 serif/sans/mono를 본교재와 동일한 Latin Modern 계열로 복원했다.
- 한글은 TeX Live bundled `UnBatang` / `UnDotum`을 사용해 시스템 폰트 의존성을 제거했다.
- theorem 계열은 본교재처럼 본문을 italic, Definition/Formula/Fact/Algorithm 계열은 upright로 구분했다.
- 주요 theorem-like heading은 `Theorem. Title` 뒤 줄바꿈을 유지하고, Remark/Strategy/Tactic만 같은 줄에 두었다.
- Chapter heading의 불필요한 장식 rule을 제거하고 본교재의 `Chapter N` + title hierarchy에 더 가깝게 정리했다.
- running header의 rule을 제거해 본문보다 header가 눈에 띄지 않도록 했다.
- 표지 metadata는 `Second Edition · 2026년 9월 17일`로 정리했다.
- 내용 파일(`MathematicalConcepts.tex`, `FactstoMemorize.tex`, `Mathematicsin2026.tex`)은 직전 43쪽 Final을 그대로 보존했다.
