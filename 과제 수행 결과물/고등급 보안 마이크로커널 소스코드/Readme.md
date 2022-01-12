# CHAOS 소스코드 빌드 & 

## ChHAOS를 ardupilot과 함께 빌드

### 환경 설정
```bash
git clone https://github.com/ArduPilot-CHAOS/ardupilot
cd ardupilot
git submodule update --init --recursive
./Tools/environment_install/install-prereqs-ubuntu.sh -y
source ~/.profile
```


### CHAOS 및 ArduPilot 컴파일
```bash
cd ardupilot
git submodule update --init --remote -- modules/ChibiOS
./waf configure --board CubeOrange
./waf copter
```

CubeOrange 대신 CubeBlack 또는 fmuv3 입력 가능  
fmuv3는 기존 drone board이다. (Pixhawk 2)  
Pixhawk2가 아닌 Pixhawk 2.1인 경우에는 fmuv3 대신 CubeBlack 또는 CubeOrange를 입력해야 함!  

./waf copter 명령은 다음 순서로 진행됨.  
CHAOS 컴파일 → ArduPilot 외부 모듈 컴파일 → ArduPilot 라이브러리 코드 컴파일  
한번 컴파일이 된 부분은 그 다음부터 컴파일을 스킵함.  
따라서, 중간에 ChibiOS 소스가 변경되었다면, 반드시 ardupilot/build 폴더를 지우고 CHAOS 컴파일부터 다시 시작해야 함.  
참고로 copter 역시 우리가 target하는 drone의 종류이다.  

## CHAOS 시뮬레이터 빌드 (ArduPilot과 별도 빌드)

드론에 올리지 않고 CHAOS의 기능 동작 여부를 시뮬레이터로 확인 가능함.  
아래 내용은 CHAOS를 ArduPilot와 컴파일 하지 않고 진행 가능함.  
CHAOS만 따로 빌드하는 방법이기도 함  

- 환경
    - Ubuntu 18.04 64bit 또는 20.04 (주의! WSL 1은 안됨 - 32 bit 프로그램 실행 불가)
    - ChibiOS 19.1.0
    - GCC 7.4.0
- 시뮬레이터 위치

```bash
cd ChibiOS/demos/various/RT-Posix-Simulator
```

- 시뮬레이터 빌드

    만약, CHAOS를 수정했다면 반드시 make를 통해 build해야 함.  
    시뮬레이터의 main.c 파일 역시, 변경된 함수나 API에 맞게 수정해야 함.  
    새로 빌드하는 경우에는 build 폴더 지우고 다시 빌드하는 것 추천

```bash
make
```

- 시뮬레이터 테스트 코드 위치

    CHAOS의 함수나 매크로 인자 등이 변경되면,
    테스트 코드에서 호출하는 함수나 매크로를 변경해야 할 수도 있음.

```bash
cd ChibiOS/demos/test/rt/source/test
```

- 시뮬레이터 실행

    build 디렉토리 내의 ch 파일이 곧 시뮬레이터 서버
    자동으로 두개의 port에 대해 listen 상태로 넘어감 (29001, 29002)

```bash
cd CHAOS/demos/various/RT-Posix-Simulator/build
./ch
```

- 시뮬레이터 접속

    다른 콘솔에서 시뮬레이터 서버에 접속해야 함

```bash
telnet 127.0.0.1 29001 # 29001 대신 29002도 가능
```

- 시뮬레이터에서 테스트 실행

    시뮬레이터 서버에 접속한 화면에서 test rt 명령어를 통해 기능 테스트
    CHAOS의 기본적인 기능에 대해 충분히 체크 가능

```bash
test rt
```

## ArduPilot → Drone (Pixhawk 2.1)

CHAOS가 포함된 ArduPilot을 드론에 올릴 때는 다음 절차를 거쳐야 함  
Drone 보드가 컴퓨터랑 USB로 연결되어있어야 함  
미리 Mission Planner나 QGroundControl 같은 프로그램을 통해서 usb driver 설치 권장  

1. ./waf configure --board fmuv3
2. ./waf copter
3. ./waf --targets bin/arducopter --upload


## Issue
make시 'sys/cdefs.h' not found 에러가 뜨면 다음과 같은 패키지 설치할 것  
```bash
sudo apt install gcc-multilib g++-multilib
```
