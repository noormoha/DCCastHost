# DCCast Host

DCCast Host is a RESTFul API program that manage [NORM](https://www.nrl.navy.mil/itd/ncs/products/norm) transmission between hosts.

## API

Defination of API can be found [here](https://alston-tang.github.io/DCCastHost/).

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

## Porgress

Currently there is only a dumb HTTP server that ehco any valid request. I will firstly implement /receive and /send.
