# FINAL-r4 변경 내역

기준: GitHub `jkpxT14/POSTECH`의 `568c78e43f1ab7a3fc206e6602f4cafede6adaae`.
에디션 기준일: **2026-09-15**. 편집일: 2026-09-16. 본교재만 변경.

기존 229문항 대조 기록에서 본교재에 필요한 보완을 반영했다. 문제 원문을 추가하지 않았으며, 아래 20개 삽입·확장이 기존 Chapter/Section 안에 들어간다. 인물·공식이 이미 있는 경우 다시 넣지 않았다.

| 위치 | 항목 | 보완 근거 | 내용 |
|---|---|---|---|
| Ch. 2 / One-Variable Calculus | Arctangent Differences and Telescoping | PS 9/14.3 | 주값과 분기 조건, 망원합으로 바꾸는 일반식 |
| Ch. 2 / One-Variable Calculus | Logarithmic Moments on the Unit Interval | PPTX 9/16.6 | 적분의 수렴 조건과 급수로의 연결 |
| Ch. 3 / Basis and Dimension 뒤 | Dimension of a Sum of Subspaces | PS 9/16.1 | 부분공간 합의 정의, 차원 공식, 교집합 하한 |
| Ch. 3 / Special Matrix Recognition Table 뒤 | Real Skew-Symmetric Matrices | PS 9/15.4 | 짝수 rank, 홀수 차수 determinant, 최대 rank 달성 |
| Ch. 3 / Eigenvalues and the Characteristic Polynomial 뒤 | Eigenvalues of Rectangular Products | PS 9/9.3 | 직사각형 AB/BA의 특성다항식과 중복도; 고유값 정의 뒤에 배치 |
| Ch. 3 / Jordan Canonical Form 뒤 | Ranks of Powers of a Nilpotent Matrix | PS 9/14.7 | Jordan 블록별 rank와 nullity 증가량 |
| Ch. 5 / Stars and Bars 뒤 | Gap Counting and Block Counting | PS 9/14.1; 9/16.4 | 비인접 선택과 지정된 원소들의 인접 배치 |
| Ch. 5 / Turán 정리 앞 | Cliques, Independent Sets, and Graph Complements | PPTX 전산·수학 교차 문항 | clique, independent set, 여그래프 정의와 대응 |
| Ch. 6 / Linearity of Expectation 뒤 | Cycles in a Uniform Random Permutation | PPTX 9/16.34 | 균등 순열에서 길이별 순환 수와 총 순환 수 기대값 |
| Ch. 8 / 기존 Vieta Jumping 내부 | Vieta Jumping | PPTX 9/14.39 | 다른 근 변환; 정수성·양수성·감소 조건 구별 |
| Ch. 8 / Sum-of-Divisors Function 뒤 | Least Integer with a Prescribed Divisor Count | PS 9/16.7 | 곱셈적 분할 전체 비교와 지수 배치 원칙 |
| Ch. 8 / Finite Fields 뒤 | Counting Invertible Matrices over a Finite Field | PS 9/13.6 | 독립 열의 선택과 GL의 위수 |
| Ch. 9 / Countability of Finitely Generated Groups 뒤 | Standard Generators of the Modular Matrix Group | Past 2021.3 | SL₂(Z)의 표준 두 생성원 |
| Ch. 9 / Subgroups and Generators of a Finite Cyclic Group 뒤 | Homomorphisms between Finite Cyclic Groups | PS 9/15.2 | 생성원의 상, 합동식, gcd 개수 |
| Ch. 9 / Viète 뒤 | Logarithmic Derivative of a Polynomial | PS 9/9.6 | 근의 역수합; 중복도와 분모 조건 |
| Ch. 10 / Fundamental Groups of Familiar Spaces 내부 | Fundamental Groups of Familiar Spaces | Past 2021.3; 리허설 2 위상 문항 | punctured disk의 자유군; 위상동형과 호모토피 구별 |
| Ch. 11 / Nash Equilibrium 뒤 | Mixed Strategies in a Two-by-Two Zero-Sum Game | PS 9/9.5 | 기대보수→무차별 조건→확률 범위·경계 전략 |
| Ch. 11 / Floor and Fractional-Part Identities 뒤 | Grouping a Floor Sum by Its Values | PS 9/14.9 | 값별 구간 분할과 불완전한 마지막 구간 |
| Part III / Ch. 16 기존 수상 표 | WLF Prize for Young Scientists | PS 9/15.3 | Ziquan Zhuang–2026 WLF–K-stability·birational geometry |
| Part III / Ch. 20 Tsimerman 기존 항목 | Jacob Tsimerman, OpenAI, and MAISI | PS 9/15.9 | MAISI Scientific Director와 기관의 연구 목적 |

## 유지 및 제외 결정

- PastProblems.tex의 기존 103문제·106개 해설 블록은 바이트 단위로 유지했다. 앞부분의 번호가 바뀐 참조는 LaTeX가 다시 계산한다.
- MATH000의 lowercase `span`, 행렬·벡터 굵은체, `\Exp`, `\rk` 등의 기존 표기를 유지했다. 새로운 국소 기호는 사용 위치에서 정의했다.
- Bessel 함수, 균등극한 정리, ζ(−1), 단위근 곱, Conway 상수, Sophus Lie 등 기존 수록 내용은 중복 추가하지 않았다.
- Collatz의 특정 최소값, 위수 2024 군의 특수 분류 숫자, 옛 사제관계·직업 상태는 본문 암기 항목으로 추가하지 않았다. 원문 오류로 지적한 PPTX 조건·답안도 교재의 정리로 흡수하지 않았다.
- Rice 정리는 전산 교차 문항의 선택 후보로 남겼다. 이번 수학 전문항 보완 범위에서 결정가능성 이론을 새로 확장하지 않았다. 여그래프 관계는 기존 그래프 정리와 바로 연결되므로 짧게 반영했다.
- Final Review, Problem sets 및 원 PPTX는 수정하지 않았다. 파이널은 이 본교재와 별도로 갱신해야 한다.

## 시사 정보의 근거

- [Stanford Mathematics, 2026-09-11](https://mathematics.stanford.edu/news/professor-ziquan-zhuang-awarded-2026-wlf-prize-young-scientists): Ziquan Zhuang의 수상 및 K-stability·birational geometry. 9월 15일 이전 발표이다.
- [MAISI 공식 Team](https://maisi.org/): Jacob Tsimerman의 Scientific Director 직책과 기관 목적. 9월 15일 Problem set에도 이 연결이 실려 있다. 공식 페이지에 공개일이 명시되지 않아 정확한 발표일을 새로 단정하지 않았다.

## 검토 범위

이 변경은 앞선 대조 결과의 보완 작업이다. 이번에 229문항 전체를 새로 독립 평가하거나 교재 전체의 무오류를 인증했다는 뜻은 아니다. 수식의 조건, 삽입 순서, 기존 구조·기출 보존, 빌드·참조 및 변경 페이지의 조판을 검증했다.
