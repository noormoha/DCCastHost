//
// Created by tang on 4/11/18.
//

#include <cstring>
#include <zconf.h>
#include "../NORM/NormSender.h"

using namespace DCCast;

#ifndef SENDER_RATE
#define SENDER_RATE 30000
#endif

int main() {
    auto *data = new char[102400];
    memset(data, 'a', 102400);
    std::cout << "Sending rate: " << SENDER_RATE << std::endl;
    NormSender sender(0, "10.0.0.2", 8888, SENDER_RATE, data, 102400);
    for (int i = 0; i < 120; i++) {
        std::cout << sender.get_progress() << " bytes has been sent" << std::endl;
        sleep(1);
    }
}
