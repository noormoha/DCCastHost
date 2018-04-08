//
// Created by tang on 4/7/18.
//

#include "../NORM/NormManager.h"

using namespace DCCast;

int main() {
    NormManager &manager = NormManager::get_instance();

    manager.send(0, "10.0.0.2", 54321, 3000);

    sleep(10);

    std::cout << "Update Rate" << std::endl;
    try {
        manager.update_rate(0, 30000);
    }
    catch (DCException &err) {
        std::cout << err.what() << std::endl;
        throw;
    }

    sleep(500);
}