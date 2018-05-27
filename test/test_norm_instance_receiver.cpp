//
// Created by tang on 4/5/18.
//

#include <unistd.h>
#include "../NORM/NormInstance.h"

using namespace DCCast;

int main() {
    NormInstance instance;
    instance.init_receiver(0, 12345);
    sleep(500);
}