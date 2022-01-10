![header](https://capsule-render.vercel.app/api?type=waving&color=0:a82da8,100:da8f00&height=230&section=header&text=This%20is%20CHAOS%20Repository%20!&fontAlign=60&fontAlignY=40&fontSize=50&fontColor=ffffff)

# ChibiOS-based High-Assurance Operating System
#### 관련 프로젝트 명: 고등급(EAL6 이상)보안 마이크로커널 개발
###### (영문 명: Development of High-Assurance(>=EAL6) Secure Microkernel)
본 과제는 ISO 15408, IEC 61508과 같은 국제표준에서 제시하는 고등급 보안 및 안전 수준을 확보한 제품의 개발 및 평가 기술을 확보하기 위해 수행되었다.
#### 과제 수행 팀
![image](C:\Users\JwKwak\Desktop\Github_picture.png)
##### 본 깃허브를 통해 공개되는 정보는 한국 정보통신기획평가원에서 기획한 "고등급(EAL6 이상)보안 마이크로커널 개발" 과제의 연구 결과물이다.
##### 연구 결과물은 크게 아래와 같이 분류된다.

- ✨과제 수행 결과물✨
  - 고등급 보안 마이크로커널 소스코드
  > 해당 디렉토리에는 국제 표준에서 권장하는 기법과 도구를 활용하여 개발된 고등급 보안마이크로커널(개발 언어: C/C++)의\
  > 소스코드가 공개되어 있다.
  - 고등급 평가제출물 (ISO 15408, IEC 61508)
  > 해당 디렉토리에는 고등급 보안 및 안전수준을 확보하는데 요구되는 국제표준 내 증거 요구사항에 따라 작성된 \
  > 고등급 평가제출물에 대한 정보가 공개되어 있다

- ✨과제 수행 내역✨
  - 요구사항
  > 보안기능, 안전기능 요구사항 도출 시, 위협 모델링(Threat Modeling) 또는 HARA(Hazard Analysis and Risk Analysis)와 같이\
  > 국제 표준에서 권장하는 기법에 따라 도출된 세부 결과가 공개되어 있습니다.
  > 보안기능, 안전기능 요구사항이 국제 표준에서 권장하는 체계적인 방법론을 활용하여 도출되는 과정에서 도출된 세부 결과물이 공개되어 있습니다.\
    > 1. 위협모델링: 해당 방법은 개발 중인 시스템에서 발생할 수 있는 다양한 보안 위협을 개발 초기 단계부터 고려함으로써 적은 비용(시간, 인력 등)만으로도 시스템의 보안 수준을 증대시킬 수 있는 체계적인 방법론으로 Data Flow Diagram 작성(자산 식별 및 분석 범위 산정), 공격라이브러리(Attack Library) 수집 및 위협 도출, 공격트리(Attack Tree) 작성, 완화대책(보안기능 요구사항) 순으로 진행된다. 이에 따라 본 디렉토리에는 고등급 보안 마이크로커널에 대한 Data Flow Diagram, 공격 라이브러리, 공격트리, 보안기능 요구사항에 대한 정보가 공개되어 있다.\
    > 2. 위험원 분석 및 위험 평가(HARA, Hazard Analysis and Risk Assessment):
  - 설계
  > 고등급 보안 및 안전수준을 확보하기 위해 국제 표준에서 요구하는 (준)정형 기법 기반의 도구를 활용한 CHAOS 세부 기능에 대한 설계 내역이 공개되어 있습니다.
  > 1. Isabelle/HOL (보안기능)
  > 2. YAKINDDU (안전기능)
  - 테스팅
  > 국제 표준에서 요구하는 테스팅 도구를 활용한 CHAOS에 대한 정적/동적 분석 결과가 공개되어 있습니다.
  > 1. Polyspace-Bugfinder (정적분석 관련 테스팅 도구)
  > 2. American Fuzzy Lop (동적분석 관련 테스팅 도구)

