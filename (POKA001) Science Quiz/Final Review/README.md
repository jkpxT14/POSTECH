# Final Review

*Mathematics for Science Quiz*  
**2026 Science War Edition**  
**2026-09-17**

2026 KAIST-POSTECH Science War 과학퀴즈 수학 파트를 위한 최종 회독용 문서입니다. 이 판은 포카전 전 마지막 Final release를 목표로 하며, 경기 직전에는 이 문서 하나만 반복해서 읽을 수 있도록 구성했습니다.

## 목적

- 한 번의 완전 회독은 약 **3시간**을 상정합니다.
- 페이지 수를 줄이는 것이 목표가 아니라, 실제 과학퀴즈에서 회수 가치가 높은 **공식, 정리, 첫 아이디어, 암기 단서, 2026 최신 수학, 선별 기출**을 높은 밀도로 담는 것이 목표입니다.
- 너무 쉬운 계산과 완전한 중복은 줄이되, 실전에서 필요한 내용은 Final이라는 이유로 과도하게 삭제하지 않습니다.
- `Facts to Memorize`는 수학사의 흐름, 주요 수학자, 저작, 상, 기관, 일화, 특수 식별 단서를 비교적 넉넉하게 유지합니다.

## 구조

1. **Part I. Mathematical Concepts** - 문제를 풀기 위해 즉시 회수해야 할 toolkit.
2. **Part II. Facts to Memorize** - 인물, 역사, 상, 저작, 기관, famous problems를 식별하기 위한 암기 파트.
3. **Part III. Mathematics in 2026** - 2025-2026 season의 주요 상, 대회, 연구, AI, 국내 수학 이슈. 2026-09-17 기준으로 최종 정리.
4. **Part IV. Selected Past Problems** - 본교재 Past Problems에서 직접 선별한 35문항. 문제와 해설은 한국어 중심으로 정리하되 원래의 수학적 조건과 문제 번호를 보존합니다.

Part / Chapter / Section 체계는 `Mathematics for Science Quiz, 2026 Science War Edition`을 기준으로 하며, 필요하지 않은 새 Chapter나 Section을 임의로 만들지 않습니다.

## 표기와 언어

- 최신 `(MATH000) Mathematical Language`의 notation과 writing convention을 최우선 기준으로 사용합니다.
- 벡터는 bold lowercase, 행렬은 bold uppercase, named operator는 upright Roman type을 사용합니다.
- 자연로그는 `\ln`, principal argument는 `\Arg z\in(-\pi,\pi]`, principal complex logarithm은 `\Log z=\ln|z|+i\Arg z`를 사용합니다.
- 위상에서는 `A^\circ` / `\operatorname{int} A`, `\overline A`, `\partial A`를 사용합니다.
- geometric distribution은 첫 성공 시행을 포함하여 셉니다.
- 문장의 골격은 한국어로 두되, 수학 전공용어, 정리명, 고유명사, 식별에 중요한 원어는 자연스러울 때 영어를 병기하거나 유지합니다.

## 검토 기반

- `Mathematics for Science Quiz, 2026 Science War Edition`
- PPTX 수학 36문항 전수 대조
- Problem sets 9세트, 90문항 전수 대조
- Past Problems 103문항 전수 대조
- Part IV 수록 대상 35문항 원문/풀이 재대조
- 최신 MATH000 notation 재대조
- 2026-09-17까지 확인한 season-specific 자료

## 빌드

XeLaTeX 기준입니다.

```bash
latexmk -xelatex FinalReview.tex
```

영문 본문은 Latin Modern 계열, 한글 본문은 TeX Live에 포함된 UnBatang/UnDotum을 사용하므로 Windows TeX Live 2026에서 별도 시스템 폰트 설치 없이 빌드할 수 있도록 구성했습니다.
