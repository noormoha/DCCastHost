//
// Created by tang on 4/4/18.
//

#include <pthread.h>
#include <cstring>
#include "NormInstance.h"

DCCast::NormInstance::NormInstance() {
    pthread_mutex_init(&mutex, nullptr);

    type = NONE;
    concrete = {};
}

void DCCast::NormInstance::init_receiver(unsigned int transferId, std::string src, unsigned short port) {
    if (concrete.receiver) {
        log_error(transferId, "An instance is already created");
        return;
    }
    this->concrete.receiver = new NormReceiver(transferId, src, port);
}

void DCCast::NormInstance::init_sender(unsigned int transferId, std::string dst, unsigned short port, uint32_t rate) {
    if (concrete.sender) {
        log_error(transferId, "An instance is already created");
        return;
    }

    // TODO SET DATA ACCORDING TO PORT
    // TEST ONLY!
    auto *data = new char[100];
    memset(data, 'a', 100);
    // TEST ONLY!

    this->concrete.sender = new NormSender(transferId, dst, port, rate, data, 100);
}


