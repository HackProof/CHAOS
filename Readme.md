![header](https://capsule-render.vercel.app/api?type=waving&color=0:a82da8,100:da8f00&height=230&section=header&text=This%20is%20CHAOS%20Repository%20!&fontAlign=60&fontAlignY=40&fontSize=50&fontColor=ffffff)

# ChibiOS-based High-Assurance Operating System Project
#### 관련 프로젝트 명: 고등급(EAL6 이상)보안 마이크로커널 개발
###### (영문 명: Development of High-Assurance(>=EAL6) Secure Microkernel)
본 과제는 ISO 15408, IEC 61508과 같은 국제표준에서 제시하는 고등급 보안 및 안전 수준을 확보한 제품의 개발 및 평가 기술을 확보하기 위해 수행되었다.

### [연구 결과물]

- ✨과제 수행 결과물✨
  - 고등급 보안 마이크로커널 소스코드<img src="https://img.shields.io/badge/-C%2FC%2B%2B-red">
  > 해당 디렉토리에는 국제 표준에서 권장하는 기법과 도구를 활용하여 개발된 고등급 보안마이크로커널(개발 언어: C/C++)의 소스코드가 공개되어 있다.
  - 고등급 평가제출물 (ISO 15408, IEC 61508)
  > 해당 디렉토리에는 고등급 보안 및 안전수준을 확보하는데 요구되는 국제표준 내 증거 요구사항에 따라 작성된 고등급 평가제출물에 대한 정보가 공개되어 있다

- ✨과제 수행 내역✨
  - 요구사항<img src="https://img.shields.io/badge/-Microsoft%20Threat%20Modeling%20Tool-brightgreen">
  > 보안기능, 안전기능 요구사항 도출 시, 위협 모델링(Threat Modeling) 또는 HARA(Hazard Analysis and Risk Analysis)와 같이
  > 국제 표준에서 권장하는 기법에 따라 도출된 세부 결과가 공개되어 있습니다.
  > 보안기능, 안전기능 요구사항이 국제 표준에서 권장하는 체계적인 방법론을 활용하여 도출되는 과정에서 도출된 세부 결과물이 공개되어 있습니다.

  1. **위협모델링**: 개발 중인 시스템에서 발생할 수 있는 다양한 보안 위협을 개발 초기 단계부터 고려함으로써 적은 비용으로 시스템의 보안 수준을 증대시킬 수 있는 체계적인 방법론

        a. Data Flow Diagram 작성 <br>
        b. 공격라이브러리(Attack Library) 수집 및 위협 도출 <br>
        c. 공격트리(Attack Tree) 작성 <br>
        d. 완화대책(보안기능 요구사항) 순으로 수행 <br>


  2. **위험원 분석 및 위험 평가(HARA, Hazard Analysis and Risk Assessment)**: 


  - 설계<img src="https://img.shields.io/badge/-Isabelle%2FHOL%20(%EB%B3%B4%EC%95%88%EA%B8%B0%EB%8A%A5)-green"> <img src="https://img.shields.io/badge/-YAKINDDU%20(%EC%95%88%EC%A0%84%EA%B8%B0%EB%8A%A5)-yellowgreen">
  > 고등급 보안 및 안전수준을 확보하기 위해 국제 표준에서 요구하는 (준)정형 기법 기반의 도구를 활용한 CHAOS 세부 기능에 대한 설계 내역이 공개되어 있습니다.

  - 테스팅<img src="https://img.shields.io/badge/-Polyspace--Bugfinder%20(%EC%A0%95%EC%A0%81%EB%B6%84%EC%84%9D%20%EA%B4%80%EB%A0%A8%20%ED%85%8C%EC%8A%A4%ED%8C%85%20%EB%8F%84%EA%B5%AC)-yellow"> <img src="https://img.shields.io/badge/-American%20Fuzzy%20Lop%20(%EB%8F%99%EC%A0%81%EB%B6%84%EC%84%9D%20%EA%B4%80%EB%A0%A8%20%ED%85%8C%EC%8A%A4%ED%8C%85%20%EB%8F%84%EA%B5%AC)-orange"> 
  > 국제 표준에서 요구하는 테스팅 도구를 활용한 CHAOS에 대한 정적/동적 분석 결과가 공개되어 있습니다.

### [연구 수행 팀 구성 및 체계]

![Github_picture](https://user-images.githubusercontent.com/31889026/148824406-e297ecf0-a840-4f5b-af56-5ae94b2fb7c6.png)

- 고려대학교 김승주교수님 연구팀 (총 8명)
  > 1. 고등급 보안 마이크로커널에서 요구하는 ISO 15408 기반 평가 제출물 개발 방법론 도출
  > 2. 고등급 보안 마이크로커널 감사, 자체보호, 자원활용 관련 보안기능 요구사항, 설계 정형 명세 및 검증
- 고려대학교 최진영교수님 연구팀 (총 5명)
  > 1. 고등급 보안 마이크로커널에서 요구하는 IEC 61508 기반 평가 제출물 개발 방법론 도출
  > 2. 고등급 보안 마이크로커널 안전기능 요구사항, 설계 정형 명세 및 검증
- 성균관대학교 김형식교수님 연구팀 (총 19명)
  > 1. 고등급 보안 마이크로커널 보안 관리, 식별 및 인증, 사용자 데이터 보호 관련 보안기능 요구사항, 설계 정형 명세 및 검증
- 연세대학교 차호정교수님 연구팀(총 10명): 
  > 1. 고등급 보안 마이크로커널 구현 및 성능 최적화 
