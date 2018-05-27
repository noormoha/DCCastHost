//
// Created by tang on 4/11/18.
//

#include <cstring>
#include <unistd.h>
#include "../NORM/NormReceiver.h"

using namespace DCCast;

#ifndef SENDER_RATE
#define SENDER_RATE 30000
#endif

int main() {
    NormReceiver receiver(0, 8888);
    timespec start_time = {}, end_time = {};
    if (!clock_gettime(CLOCK_REALTIME, &start_time)) {
        std::cerr << "Can not get time" << std::endl;
    }
    for (int i = 0; i < 60; i++) {
        std::cout << receiver.get_progress() << " bytes has been sent" << std::endl;
        sleep(1);
    }
    if (clock_gettime(CLOCK_REALTIME, &end_time) != 0) {
        std::cerr << "Can not get time" << std::endl;
    }
    double diff = end_time.tv_sec - start_time.tv_sec +
            static_cast<double >(end_time.tv_nsec - start_time.tv_nsec) / 1000000000;
    std::cout << "Receiving rate: " << receiver.get_progress() / diff / 1000000 * 8;
}
