# Mathematics for Science Quiz — Final Review

2026 KAIST–POSTECH Science War 과학퀴즈 수학 파트를 위한 **최종 회독용 문서**입니다.

## 목적

- 포카전 전날부터 경기 직전까지 반복해서 읽을 수 있는 분량과 밀도를 목표로 합니다.
- 한 번의 완전 회독은 대략 **2–3시간**을 상정합니다.
- 본교재 전체를 다시 읽는 문서가 아니라, 문제를 보았을 때 필요한 **공식·정리·first idea·암기 단서**를 빠르게 회수하는 문서입니다.
- 지나치게 쉬운 기본 계산과 일회성 퍼즐은 줄이되, 실제 출제 가치가 있는 개념과 암기 정보는 충분히 보존합니다.
- 특히 `Facts to Memorize`는 수학사의 흐름, 주요 수학자·저작·상·기관·일화·특수 식별 단서를 비교적 넉넉하게 수록합니다.

## 구조와 기준

- `Mathematics for Science Quiz, 2026 Science War Edition`의 Part / Chapter / Section 흐름을 따릅니다.
- MATH000과 본교재의 표기·용어·수학적 convention을 따릅니다.
- 본교재의 theorem-like 시각 문법을 반영하여 `Theorem. Title`, `Formula. Title`, `Fact. Title` 등의 heading 뒤에 본문을 줄바꿈하여 배치합니다.
- 한국어 문서를 기본으로 하되, 표준 수학 용어·고유명사·원제는 식별에 유리할 때 영어를 병기합니다.

## 검토 기반

- 본교재 FINAL-r4
- PPTX 수학 36문항 전수 대조
- Problem sets 9세트, 90문항 전수 대조
- Past Problems 103문항 전수 대조
- 2026-09-15 cutoff의 본교재 Part III 및 프로젝트 자료
- 2026-09-17까지의 Final Review 관련 프로젝트 결정사항

## 빌드

XeLaTeX 기준입니다.

```bash
latexmk -xelatex FinalReview.tex
```

최종 PDF는 렌더링·전페이지 시각 검토와 PDF preflight를 거쳤습니다.
