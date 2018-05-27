//
// Created by tang on 4/7/18.
//

#include <unistd.h>
#include "../NORM/NormManager.h"

using namespace DCCast;

int main () {
    try {
        NormManager &manager = NormManager::get_instance();

        std::cout << &manager << std::endl;
        NormManager &n = NormManager::get_instance();
        std::cout << &n << std::endl;

        manager.receive(0, 54321);

        int count = 0;
        while (count < 35) {
            std::cout << manager.get_progress(0) << std::endl;
            sleep(1);
            count++;
        }
        manager.terminate(0);

        count = 0;
        sleep(3);
        manager.receive(0, 54321);
        while (count < 35) {
            std::cout << manager.get_progress(0) << std::endl;
            sleep(1);
            count++;
        }
    }
    catch (DCException &e) {
        std::cerr << e.what() << std::endl;
    }
}