# CHAOS Project

## How to build CHAOS with ArduPilot

### Set up the configuration
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

The example above shows how to build CHAOS for the `CubeOrange` board with ArduPilot.  
The `copter` is the type of our drones.  

You can write `CubeBlack` or `fmuv3` instead of the `CubeOrange` keyword.  
`fmuv3` is for the original Pixhawk2 Cube board, `CubeBlack` for Pixhawk2.1 Cube black, and `CubeOrange` for Pixhawk2.1 Cube Orange.  
If you'd like to see other available boards, please use the command below:  
```
./waf list_boards
```

The `./waf copter` command performs the following processes:  
(1) Compiling CHAOS  
(2) Compiling ArduPilot modules  
(3) Compiling ArduPilot libraries  

Please note that the Waf build system will skip the processes which are successfully compiled before.  
So, if some codes of CHAOS are modified, you should remove the `ardupilot/build` directory before running the Waf build command.  
Or, you can use the `./waf clean` command.


## How to build and run CHOAS without ArduPilot
You can build CHAOS without ArduPilot.  
In this case, a simulator or a demo application run on the CHAOS kernel.  

### Set up the configuration
- Ubuntu 18.04 64-bit or 20.04 64-bit
    -  Note: WSL 1 must be avoided, as the WSL cannot run 64-bit programs.
- GCC 7.4.0 or higher versions

    
### Build
```bash
cd ChibiOS/demos/various/RT-Posix-Simulator
make
```
Please note that this simulator refers to the `ChibiOS/demos/test/rt/source/test` directory.  


### Run
```bash
cd CHAOS/demos/various/RT-Posix-Simulator/build
./ch
```

The simulator listens to two ports: 29001, 29002.  
You can access the running simulator with the below command.
```bash
telnet 127.0.0.1 29001 # Also, 'telnet 127.0.0.1 29002' is OK
```


## How to upload ArduPilot to drones (Pixhawk 2.1)

You can use the Waf build system to upload the ArduPilot binary, which is compiled with CHAOS, to drones.  
It is highly recommended that you install `Mission Planner` or `QGroundControl` before uploading the ArduPilot, because these programs will install relevant drivers on your computers.  

First, connect your drones to computers via USB.  
Then, run the following commands.  
```bash
./waf configure --board fmuv3
./waf copter
./waf --targets bin/arducopter --upload
```


## Issues
If you run into various errors like `'sys/cdefs.h' not found`, you might have to install the following packages.  
```bash
sudo apt install gcc-multilib g++-multilib
```
