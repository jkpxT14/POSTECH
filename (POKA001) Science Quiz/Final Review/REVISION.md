# 2026 Science War Edition - Final Release

Edition date: **2026-09-17**

이 revision은 2026 KAIST-POSTECH Science War 전 마지막 Final Review release를 목적으로 한다. v2.3에서 확립한 내용 breadth와 typography를 모체로 삼고, 새로운 실험보다 누락, 오류, 불일치, 번역투, 조판 문제를 제거하는 데 초점을 맞췄다.

## Final identity

- Main title: `Final Review`
- Subtitle: `Mathematics for Science Quiz`
- Edition: `2026 Science War Edition`
- Date: `2026년 9월 17일`
- 목표 회독 시간: 약 3시간
- 페이지 수를 줄이기 위해 중요한 내용을 삭제하지 않는다.

## Content pass

- v2.3의 고밀도 Part I-III 내용을 기본적으로 보존했다.
- 기존 전수 audit에서 보강된 finite-field power map, coefficient extraction, Fibonacci generating function, Conjugate Root Theorem, Cavalieri principle, Kummer carry interpretation, finitely generated group countability, Möbius/fractional-linear recurrence 등의 핵심을 유지했다.
- Final robustness를 위해 기존 Chapter/Section 안에만 다음 고효율 결과를 짧게 보강했다.
  - Power of a Point
  - Ptolemy's Theorem
  - LTE의 홀수 소수 핵심형
  - Lucas' Theorem
  - Rational Root Theorem / Eisenstein Criterion
- 과거 audit에서 배제한 불안정하거나 잘못된 항목은 다시 추가하지 않았다.

## Part IV - Selected Past Problems

본교재 Past Problems에서 사용자가 직접 선별한 다음 35문항을 새 Part IV에 수록했다.

`2011.7, 2011.8, 2014.3, 2014.4, 2015.2, 2015.3, 2015.6, 2015.12, 2015.17, 2017.2, 2017.3, 2018.3, 2019.2, 2020.2, 2020.4, 2021.2, 2021.3, 2021.4, 2022.1, 2022.3, 2022.7, 2023.2, 2023.3, 2023.4, 2024.3, 2024.4, 2025.1, 2025.3, 2025.4, 2025.5, 2025.6, 2025.8, 2025.13, 2025.17, 2025.19`

- 문제 번호, 조건, 수치, 선택지의 수학적 의미를 본교재와 대조했다.
- 문제와 해설은 한국어 중심으로 다시 작성했다.
- 해설은 정답만 남기지 않고 경기 직전 풀이가 복원될 만큼의 핵심 논리를 유지했다.
- 2024.3의 도형은 본교재 figure를 그대로 포함했다.
- 임의의 추가 기출이나 임의 삭제는 하지 않았다.

## Korean-language pass

- 문장 골격을 한국어로 두고, 표준 수학 용어와 식별에 유리한 영어는 필요한 곳에서 유지했다.
- 최신 수학의 `published / accepted / preprint / announced / formally verified / scheduled` 상태 구분을 한국어 설명 중심으로 정리했다.
- Part IV의 문제/해설에서 직역투를 줄이고 자연스러운 한국어 수학 문장으로 통일했다.

## 2026 current-math pass

- Part III cutoff를 **2026-09-17**로 갱신했다.
- Navier-Stokes는 2026-09-08 공개된 solution manuscript와 Lean formalization까지를 확인된 상태로 표현하고, ordinary publication / Clay 공식 인정 / credit determination을 별도 주장으로 분리했다.
- PrimeGaps186은 세 개의 명시적 project input axiom과 numerical certificate에 의존하는 conditional formalization이라는 점을 명확히 했다.
- Palomar는 2026-08-18 submissions 개시와 Lean FRO / ICARM incubated registry라는 상태를 반영했다.
- `Ten AI-Generated Research Advances`의 모델 표현은 공개 근거보다 강한 특정 모델명 귀속을 피하고 `OpenAI 내부 연구 시스템`으로 정정했다.
- Cycle Double Cover 항목에는 공개 Lean formalization의 존재를 반영하되 ordinary journal publication과 구별했다.

## MATH000 sync

최신 GitHub `(MATH000) Mathematical Language`를 다시 대조했다.

- vectors: bold lowercase; matrices: bold uppercase
- natural logarithm: `\ln`; arbitrary-base logarithm: `\log_a`
- principal argument: `\Arg z\in(-\pi,\pi]`
- principal complex logarithm: `\Log z=\ln|z|+i\Arg z`
- interior / closure / boundary: `A^\circ` / `\operatorname{int} A`, `\overline A`, `\partial A`
- probability/expectation: blackboard-bold convention
- geometric distribution: first successful trial included
- least common multiple: upright `lcm` operator
- indicator symbol: dsfont `\mathds{1}` when available, safe fallback otherwise

## Typography and build

- v2.3에서 확립한 Latin Modern + UnBatang/UnDotum 조합을 유지했다.
- 본교재의 theorem-like grammar를 유지했다: main theorem/fact/formula heading 뒤 줄바꿈, Remark/Idea/Strategy/Tactic은 same-line.
- Part IV의 Problem / Solution / Hint / choices 환경을 본교재의 visual grammar에 맞게 추가했다.
- 불필요한 장식이나 새 Chapter/Section을 만들지 않았다.
- 배포 ZIP에는 source, figure, PDF, README/REVISION, `.latexmkrc`만 포함하고 build artifacts는 제외한다.
