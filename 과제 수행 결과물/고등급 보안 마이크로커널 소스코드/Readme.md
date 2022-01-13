# CHAOS Project

## How to build CHAOS with ArduPilot

### Configuration
```bash
git clone https://github.com/ArduPilot-CHAOS/ardupilot
cd ardupilot
git submodule update --init --recursive
./Tools/environment_install/install-prereqs-ubuntu.sh -y
source ~/.profile
```

### Build CHAOS with ArduPilot
```bash
cd ardupilot
git submodule update --init --remote -- modules/ChibiOS
./waf configure --board CubeOrange
./waf copter
```

The example above shows how to build CHAOS for the `CubeOrange` board with ArduCopter from ArduPilot.  
The `copter` is the type of our drones.  

You can write `CubeBlack` or `fmuv3` instead of the `CubeOrange` keyword.  
Just for your information, `fmuv3` is for the original Pixhawk2 Cube board, `CubeBlack` for Pixhawk2.1 Cube black, and `CubeOrange` for Pixhawk2.1 Cube Orange.  
If you'd like to see other available boards, please use the command below:  
```
./waf list_boards
```

The `./waf copter` command does the below processes:  
(1) Compiling CHAOS  
(2) Compiling ArduPilot module  
(3) Compiling ArduPilot libraries  

Please note that the Waf build system will skip the processes which are successfully compiled before.  
So, if some codes of CHAOS are modified, you should remove the `ardupilot/build` directory before running the Waf build command.  


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
