//
// Created by tang on 4/4/18.
//


#include <cstring>
#include <zconf.h>
#include "../NORM/NormReceiver.h"

using namespace DCCast;

int main() {
    NormReceiver receiver(0, "0.0.0.0", 19242);
    int count = 0;
    while (count < 20) {
        std::cout << receiver.progress << std::endl;
        sleep(1);
        count++;
    }

    DCCommand command = {};
    command.id = 0;
    command.type = TERMINATE;

    receiver.requests->enqueue(command);

    DCResponse response = {};

    receiver.responses->wait_dequeue(response);

    std::cout << "Terminate" << std::endl;

    sleep(5);

    std::cout << receiver.status << std::endl;
}
