# DCCast Host

DCCast Host is a RESTFul API program that manage [NORM](https://www.nrl.navy.mil/itd/ncs/products/norm) transmission between hosts.

## API

Definition of API can be found [here](https://alston-tang.github.io/DCCastHost/).

## Compilation and Running

DCCast Host relies on [Pistache](http://pistache.io/), which is a C++ REST HTTP server framework and [NRL NORM Implementation](https://downloads.pf.itd.nrl.navy.mil/norm/), which implment and abstract NORM protocol. To compile DCCast Host, you need to compile and install above pre-requisite first.

Use cmake to compile DCCast Host.
```shell
mkdir build
cd build
cmake ..
make
```
Once you compile DCCast Host, you can find a executable file DCCastHost in your build directory. Execute this file will create a HTTP server listen at port 9080.

## Test

DCCast Host comes with a simple test script. To run test yourself, you need to first run DCCastHost on at least two hosts.

Then modify global variable *test_topo* in test/test.py corresponding to the topology you set at previous step.
*test_topo* is an array of links between two hosts. A link in *test_topo* array is an array that contain four elements:
1. ID of host1 (index 0)
2. ID of host2 (index 1)
3. IP address of host1's interface that the link connected with (index 2)
4. IP address of host2's interface that the link connected with (index 3)

Finally, run test.py at a place where RESTFul request can be sent to all hosts that you set at first step.
```shell
python3 test.py
```
or
```shell
python test.py
```
if python 3 is the default interpreter of python.

## Progress

All APIs listed on the definition page have been implemented.

## Caveat

Noted that *cleanAll* will terminated all transmissions but will not completely recycle the memory space used to store the terminated transmission.
The space used by the program is positively related to the number of transmission IDs that are used no matter whether the corresponding transmission is active or not. However, a terminated transmission will only take less than 50 bytes memory space and this space will be reused once the transmission ID of the terminated transmission is reused by another active transmission.