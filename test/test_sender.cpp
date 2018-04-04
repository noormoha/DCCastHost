//
// Created by tang on 4/4/18.
//

#include <cstring>
#include <zconf.h>
#include "../NORM/NormReceiver.h"
#include "../NORM/NormSender.h"

using namespace DCCast;

int main() {
    auto *data = new char[1000];
    memset(data, 'a', 1000);
    std::cout << sizeof(data);
    NormSender sender(0, "10.0.0.2", 19242, 2000, data, 1000);

    sleep(10000);
}
