//
// Created by tang on 4/7/18.
//

#include "../NORM/NormManager.h"

using namespace DCCast;

int main () {
    NormManager &manager = NormManager::get_instance();

    std::cout << &manager << std::endl;
    NormManager &n = NormManager::get_instance();
    std::cout << &n << std::endl;

    manager.receive(0, 54321);

    sleep(500);
}