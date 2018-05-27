//
// Created by tang on 4/7/18.
//

#include <unistd.h>
#include "../NORM/NormManager.h"

using namespace DCCast;

int main() {
    NormManager &manager = NormManager::get_instance();

    manager.send(0, "10.0.0.2", 54321, 3000);

    int count = 0;
    while (count < 10) {
        std::cout << manager.get_progress(0) << std::endl;
        sleep(1);
        count++;
    }

    manager.update_rate(0, 300000);

    count = 0;
    while (count < 10) {
        std::cout << manager.get_progress(0) << std::endl;
        sleep(1);
        count++;
    }

    manager.terminate(0);

    count = 0;
    while (count < 10) {
        std::cout << manager.get_progress(0) << std::endl;
        sleep(1);
        count++;
    }

    count = 0;
    manager.send(0, "10.0.0.2", 54321, 30000);
    while (count < 10) {
        std::cout << manager.get_progress(0) << std::endl;
        sleep(1);
        count++;
    }

}