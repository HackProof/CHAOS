
# ChibiOS-based High-Assurance Operating System Project
## 프로젝트 개요
####  &#160; &#160; 1. 관련 프로젝트 명
 &#160; &#160; &#160; &#160; &#160; &#160;고등급(EAL6 이상)보안 마이크로커널 개발 (Development of High-Assurance(>=EAL6) Secure Microkernel)
####  &#160; &#160;2. 프로젝트 수행 목적
 &#160; &#160; &#160; &#160; &#160; &#160;고등급(EAL6 이상) 보안 마이크로커널 개발 및 고등급 평가에서 요구되는 주요 평가 제출물 개발 방법론 도출
####  &#160; &#160;3. 프로젝트 수행팀
 &#160; &#160; &#160; &#160; &#160; &#160;고려대학교 김승주교수님 연구팀 &#160; (총 8명) <br>
 &#160; &#160; &#160; &#160; &#160; &#160;- 고려대학교 최진영교수님 연구팀 &#160; (총 5명) <br>
 &#160; &#160; &#160; &#160; &#160; &#160;- 성균관대학교 김형식교수님 연구팀 &#160; (총 19명) <br>
 &#160; &#160; &#160; &#160; &#160; &#160;- 연세대학교 차호정교수님 연구팀 &#160; (총 10명) <br>

### [연구 결과물]

- ✨과제 수행 결과물✨
  - [고등급 보안 마이크로커널 소스코드](https://github.com/HackProof/CHAOS/tree/main/%EA%B3%BC%EC%A0%9C%20%EC%88%98%ED%96%89%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C) &#160; <img src="https://img.shields.io/badge/-C%2FC%2B%2B-red">
  > 국제 표준에서 권장하는 기법과 도구를 활용하여 개발된 드론용 고등급 보안마이크로커널의 소스코드
  - [고등급 평가제출물](https://github.com/HackProof/CHAOS/tree/main/%EA%B3%BC%EC%A0%9C%20%EC%88%98%ED%96%89%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC)
  > 국제표준에서 고등급 보안 및 안전수준을 확보하는데 요구하는 사항에 따라 작성된 평가문서

- ✨과제 수행 내역✨
  - 요구사항 &#160; <img src="https://img.shields.io/badge/-Microsoft%20Threat%20Modeling%20Tool-brightgreen">
  > 위협 모델링(Threat Modeling) 또는 HARA(Hazard Analysis and Risk Analysis)와 같은 방법론을 활용한 CHAOS 보안기능/안전기능 요구사항

  - 설계 &#160; <img src="https://img.shields.io/badge/-Isabelle%2FHOL%20(%EB%B3%B4%EC%95%88%EA%B8%B0%EB%8A%A5)-green"> <img src="https://img.shields.io/badge/-YAKINDDU%20(%EC%95%88%EC%A0%84%EA%B8%B0%EB%8A%A5)-yellowgreen">
  > [Isabelle/HOL](https://isabelle.in.tum.de/)과 [YAKINDDU](https://www.itemis.com/en/yakindu/state-machine/)를 활용한 CHAOS 보안기능/안전기능 설계
  - 테스팅 &#160; <img src="https://img.shields.io/badge/-Polyspace--Bugfinder%20-yellow"> <img src="https://img.shields.io/badge/-American%20Fuzzy%20Lop(AFL)%20-orange"> 
  > [Polyspace Bugfinder](https://kr.mathworks.com/products/polyspace-bug-finder.html)와 [AFL](https://github.com/google/AFL)을 활용한 CHAOS에 대한 테스팅 결과 <br>
  > 실 환경에서의 정상 동작 여부를 점검하기 위해 자체 테스트 스크립트를 활용한 드론 테스팅 결과



