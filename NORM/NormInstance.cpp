//
// Created by tang on 4/4/18.
//

#include <pthread.h>
#include <cstring>
#include "NormInstance.h"

namespace DCCast {

DCCast::NormInstance::NormInstance() {
    pthread_mutex_init(&mutex, nullptr);

    type = NONE;
    concrete = {};
}

void DCCast::NormInstance::init_receiver(unsigned int transferId, unsigned short port) {
    // Instance is actively used by a receiver/sender
    if (!recycle()) return;

    this->concrete.receiver.receiver = new NormReceiver(transferId, port);
}

void DCCast::NormInstance::init_sender(unsigned int transferId, std::string dst, unsigned short port, uint32_t rate) {
    // Instance is actively used by a receiver/sender
    if (!recycle()) return;

    // Construct the fake data to be sent
    char *data = new char[NORM_SENDER_DATA_OBJECT_LEN];
    for (int i = 0; i + 2 <= NORM_SENDER_DATA_OBJECT_LEN; i += 2) {
        *((unsigned short *) (data + i)) = port;
    }

    this->concrete.sender.data = data;
    this->concrete.sender.data_len = NORM_SENDER_DATA_OBJECT_LEN;
    this->concrete.sender.sender = new NormSender(transferId, dst, port, rate, this->concrete.sender.data,
                                                  this->concrete.sender.data_len);
}

bool DCCast::NormInstance::recycle() {
    dc_status status;
    switch (type) {
        case NONE:
            return true;
        case RECEIVER:
            if (!this->concrete.receiver.receiver) return true;
            status = this->concrete.receiver.receiver->get_status();

            // We can safely forget last receiver thread
            if (status == ERROR || status == TERMINATED) {
                this->concrete = {};
                this->type = NONE;
                return true;
            }

            // Receiver is running. Need to terminate it first
            return false;
        case SENDER:
            if (!this->concrete.sender.sender) return true;
            status = this->concrete.sender.sender->get_status();

            // We can safely forget last sender thread
            if (status == ERROR || status == TERMINATED) {
                this->concrete = {};
                this->type = NONE;
                return true;
            }

            // Sender is running. Need to terminate it first
            return false;
    }

    // Should not come to this place
    return false;
}

}


