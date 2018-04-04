//
// Created by tang on 4/4/18.
//

#ifndef DCCASTHOST_NORMINSTANCE_H
#define DCCASTHOST_NORMINSTANCE_H

#include <zconf.h>
#include "NormDefinition.h"
#include "NormSender.h"
#include "NormReceiver.h"

namespace DCCast {

typedef enum DC_INSTANCE_TYPE {
    SENDER,
    RECEIVER,
    NONE
} dc_instance_type;

union InstanceConcrete {
    NormSender *sender;
    NormReceiver *receiver;
};

class NormInstance {
    pthread_mutex_t mutex;
    dc_instance_type type;
    InstanceConcrete concrete;

    NormInstance();

    void init_receiver(unsigned int transferId, std::string src, unsigned short port);
    void init_sender(unsigned int transferId, std::string dst, unsigned short port, uint32_t rate);
};

}


#endif //DCCASTHOST_NORMINSTANCE_H
