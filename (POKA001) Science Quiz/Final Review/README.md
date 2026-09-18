# Final Review

*Mathematics for Science Quiz*  
**2026 Science War Edition**  
**2026-09-17**

2026 KAIST-POSTECH Science War 과학퀴즈 수학 파트를 위한 경기 직전 최종 회독 문서입니다. 최신 GitHub `Final Review`를 기준으로 본교재의 구조와 디자인 언어, `(MATH000) Mathematical Language`의 표기를 다시 맞추고, 약 3시간 안에 한 번 완독할 수 있는 고밀도 Final을 목표로 정제했습니다.

## 구성 원칙

- Part I-III의 Chapter / Section 구조는 본교재 `Mathematics for Science Quiz, 2026 Science War Edition`을 따릅니다.
- 중요한 내용을 페이지 수 때문에 삭제하지 않되, 중복 설명·메타 문장·불필요한 소제목과 theorem-like environment는 줄였습니다.
- Part I은 과학퀴즈 수준에서 실제로 쓰는 결과·공식과 함께, 문제의 형태를 보고 어떤 도구부터 확인할지 짧은 사고 흐름을 담았습니다.
- Part II는 출제 가능성이 있는 인물·상·기관·수학사·저작·일화·유명 문제를 압축하되, 시간축이 있는 내용은 연대순을 기본으로 배열했습니다.
- Part III는 2026-09-17을 기준일로 하며 발표, 프리프린트, 형식검증, 동료평가, 정식 게재를 구별합니다.
- Part IV는 사용자가 선별한 35개 기출만 수록하며, 문제의 수학적 조건을 보존하고 해설은 핵심 발상이 먼저 보이도록 다시 다듬었습니다.

## 언어와 표기

이 문서는 한국어 교재입니다. 표준적인 한국어 수학 용어·수학자명·정리명·문헌명이 있으면 이를 우선하고, 식별 가치가 있는 원어는 필요한 경우에만 병기합니다. 수학 표기는 최신 `(MATH000) Mathematical Language`를 기준으로 하며, 특히 벡터/행렬 굵기, `\rk`, `\tr`, `\im`, `\proj`, `D\mathbf F`, `\Jac`, `\Hess`, `\Prob`, `\Exp`, `\Arg`, `\Log`, `A^\circ`, `\overline A`, `\partial A` 등의 규약을 문서 전체에 적용합니다.

## 빌드

XeLaTeX 기준입니다.

```bash
latexmk -xelatex FinalReview.tex
```

영문은 Latin Modern, 한글은 TeX Live에 포함된 UnBatang / UnDotum을 사용합니다.
