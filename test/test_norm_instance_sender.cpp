//
// Created by tang on 4/4/18.
//

#include "../NORM/NormInstance.h"

using namespace DCCast;

int main() {
    NormInstance instance;
    instance.init_sender(0, "10.0.0.2", 12345, 30000);
    sleep(500);
}