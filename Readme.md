## The CHAOS(ChibiOS-based High-Assurance Operating System) Project
### 프로젝트 개요
####  &#160; &#160; 1. 관련 프로젝트 명
 &#160; &#160; &#160; &#160; &#160; &#160;고등급(EAL6 이상)보안 마이크로커널 개발 (Development of High-Assurance(>=EAL6) Secure Microkernel) <br> <br>
 &#160; &#160; &#160; &#160; &#160; &#160; ※ 이 프로젝트는 정부(과학기술정보통신부)의 재원으로 정보통신기술평가원의 지원을 받아 수행된 연구임 <br> 
 &#160; &#160; &#160; &#160; &#160; &#160; &#160; &#160; (No.2018-0-00532,고등급(EAL6 이상) 보안 마이크로커널 개발)<br>
####  &#160; &#160;2. 프로젝트 수행 목적
 &#160; &#160; &#160; &#160; &#160; &#160;고등급(EAL6 이상) 보안 마이크로커널 개발 및 고등급 평가에서 요구되는 주요 평가제출물 작성 방법론 도출
 <br>
 <br>![그림1](https://user-images.githubusercontent.com/31889026/149855984-2b56ea77-bbd5-413c-bce0-3710c4ec8ce6.png)

####  &#160; &#160;3. 프로젝트 수행팀
 &#160; &#160; &#160; &#160; &#160; &#160;- [고려대학교 김승주교수님 연구팀](https://sites.google.com/view/seceng) &#160; (총 8명) <br>
 &#160; &#160; &#160; &#160; &#160; &#160;- [고려대학교 최진영교수님 연구팀](https://sites.google.com/korea.ac.kr/fmlab) &#160; (총 5명) <br>
 &#160; &#160; &#160; &#160; &#160; &#160;- [성균관대학교 김형식교수님 연구팀](https://seclab.skku.edu/) &#160; (총 19명) <br>
 &#160; &#160; &#160; &#160; &#160; &#160;- [연세대학교 차호정교수님 연구팀](https://mobed.yonsei.ac.kr/) &#160; (총 10명) <br>

### 프로젝트 1단계 연구 결과물

- ✨과제 수행 결과물✨
  - [고등급 보안 마이크로커널 소스코드](https://github.com/HackProof/CHAOS/tree/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%EC%86%8C%EC%8A%A4%EC%BD%94%EB%93%9C) &#160; <img src="https://img.shields.io/badge/-C%2FC%2B%2B-red">
  > 국제 표준에서 권장하는 기법과 도구를 활용하여 개발된 드론용 고등급 보안마이크로커널의 소스코드
  - [고등급 평가제출물](https://github.com/HackProof/CHAOS/tree/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC)
  > 국제표준에서 고등급 보안 및 안전수준을 확보하는데 요구하는 사항에 따라 작성된 평가문서
  - 주요 특허 및 논문
  > 과제를 수행하며 출원 및 등록된 특허와 논문리스트



- ✨과제 세부 수행 내역✨
  - 요구사항 &#160; <img src="https://img.shields.io/badge/-Microsoft%20Threat%20Modeling%20Tool-brightgreen">
  > [위협 모델링(Threat Modeling)](https://docs.microsoft.com/ko-kr/azure/security/develop/threat-modeling-tool-releases-73002061) 또는 HARA(Hazard Analysis and Risk Analysis)와 같은 방법론을 활용한 CHAOS 기능 요구사항 도출 내역 <br>
&#160;&#160;&#160;&#160; **[참조할 평가제출물]** <br>
&#160;&#160;&#160;&#160;&#160;&#160; 1. [[보안성] 보안목표명세서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20%EB%B3%B4%EC%95%88%EB%AA%A9%ED%91%9C%EB%AA%85%EC%84%B8%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 2. [[보안성] 보안정책모델 문서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20%EB%B3%B4%EC%95%88%EC%A0%95%EC%B1%85%EB%AA%A8%EB%8D%B8%EB%AC%B8%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 3. [[안전성] SW 요구사항 안전성 분석보고서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EC%95%88%EC%A0%84%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20SW_REQ-%EC%95%88%EC%A0%84%EC%84%B1%EB%B6%84%EC%84%9D%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 4. [[안전성] SW 안전기능 요구사항 명세서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EC%95%88%EC%A0%84%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20SW%EC%9D%B8%EC%A0%84%EA%B8%B0%EB%8A%A5%EC%9A%94%EA%B5%AC%EC%82%AC%ED%95%AD%EB%AA%85%EC%84%B8%EC%84%9C.pdf) <br>
  - 설계 &#160; <img src="https://img.shields.io/badge/-Isabelle%2FHOL%20(%EB%B3%B4%EC%95%88%EA%B8%B0%EB%8A%A5)-green"> <img src="https://img.shields.io/badge/-YAKINDDU%20(%EC%95%88%EC%A0%84%EA%B8%B0%EB%8A%A5)-yellowgreen"> <br>
  > [Isabelle/HOL](https://isabelle.in.tum.de/)과 [YAKINDDU](https://www.itemis.com/en/yakindu/state-machine/)를 활용한 CHAOS 설계 내역 <br>
&#160;&#160;&#160;&#160;&#160; **[참조할 평가제출물]** <br>
&#160;&#160;&#160;&#160;&#160;&#160;&#160; 1. [[보안성] 기능명세서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20%EA%B8%B0%EB%8A%A5%EB%AA%85%EC%84%B8%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 2. [[보안성] 보안아키텍처 문서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20%EB%B3%B4%EC%95%88%EC%95%84%ED%82%A4%ED%85%8D%EC%B3%90%EB%AC%B8%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 3. [[보안성] TOE 설계서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20TOE%20%EC%84%A4%EA%B3%84%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 4. [[안전성] SW 안전기능 설계명세서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EC%95%88%EC%A0%84%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20SW%EC%95%88%EC%A0%84%EA%B8%B0%EB%8A%A5%EC%84%A4%EA%B3%84%EB%AA%85%EC%84%B8%EC%84%9Cv2.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 5. [[안전성] SW 설계 안전성 분석보고서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EC%95%88%EC%A0%84%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20SW%EC%84%A4%EA%B3%84%EC%95%88%EC%A0%84%EC%84%B1%EB%B6%84%EC%84%9D%EB%B3%B4%EA%B3%A0%EC%84%9Cv2.pdf) <br>
  - 테스팅 &#160; <img src="https://img.shields.io/badge/-Polyspace--Bugfinder%20-yellow"> <img src="https://img.shields.io/badge/-American%20Fuzzy%20Lop(AFL)%20-orange"> 
  > [Polyspace Bugfinder](https://kr.mathworks.com/products/polyspace-bug-finder.html)와 [AFL](https://github.com/google/AFL)을 활용한 CHAOS에 대한 테스팅 결과와 실 환경에서의 정상 동작 여부를 점검하기 위해 자체 테스트 스크립트를 활용한 테스팅 결과 <br>
&#160;&#160;&#160;&#160; **[참조할 평가제출물]** <br>
&#160;&#160;&#160;&#160;&#160;&#160; 1. [[보안성] 시험서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20%EC%8B%9C%ED%97%98%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 2. [[안전성] SW 단위 시험계획서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EC%95%88%EC%A0%84%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20SW%EB%8B%A8%EC%9C%84%EC%8B%9C%ED%97%98%EA%B3%84%ED%9A%8D%EC%84%9Cv2.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 3. [[안전성] SW 코드 안전성 분석 보고서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EC%95%88%EC%A0%84%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20SW%EC%BD%94%EB%93%9C%EC%95%88%EC%A0%84%EC%84%B1%EB%B6%84%EC%84%9D%EB%B3%B4%EA%B3%A0%EC%84%9Cv2.pdff) <br>

  - 개발환경 및 활용도구 <br>
  > &#160;&#160;&#160;&#160;&#160;&#160; 1. [[보안성] 개발보안 문서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20%EA%B0%9C%EB%B0%9C%EB%B3%B4%EC%95%88%EB%AC%B8%EC%84%9C.pdf) <br>
&#160;&#160;&#160;&#160;&#160;&#160; 2. [[보안성] 도구와 기법 문서](https://github.com/HackProof/CHAOS/blob/main/%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8%201%EB%8B%A8%EA%B3%84%20%EC%97%B0%EA%B5%AC%20%EA%B2%B0%EA%B3%BC%EB%AC%BC/%EA%B3%A0%EB%93%B1%EA%B8%89%20%EB%B3%B4%EC%95%88%20%EB%A7%88%EC%9D%B4%ED%81%AC%EB%A1%9C%EC%BB%A4%EB%84%90%20%ED%8F%89%EA%B0%80%EC%A0%9C%EC%B6%9C%EB%AC%BC/%EB%B3%B4%EC%95%88%EC%84%B1/%5B%EA%B8%B0%EC%88%A0%EB%AC%B8%EC%84%9C%5D%20%EB%8F%84%EA%B5%AC%EC%99%80%20%EA%B8%B0%EB%B2%95%20%EB%AC%B8%EC%84%9C.pdf) <br>
  - 성능치 비교 <br>
  > CHAOS는 기존에 보고된 취약점 이외에도 최신 보안 취약점을 고려하여 개발되었기 때문에, **기존의 고등급 보안 마이크로커널보다 더 많은 보안기능 요구사항을 만족** <br>
<br>![그림7](https://user-images.githubusercontent.com/31889026/150329582-e119d15b-c58b-4f04-993a-550b1c8c9982.png)
