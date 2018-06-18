//
// Created by tang on 4/4/18.
//

#ifndef DCCASTHOST_NORMINSTANCE_H
#define DCCASTHOST_NORMINSTANCE_H

#include <mutex>
#include "NormDefinition.h"
#include "NormSender.h"
#include "NormReceiver.h"

namespace DCCast {

typedef enum DC_INSTANCE_TYPE {
    SENDER,
    RECEIVER,
    NONE
} dc_instance_type;

static std::string dc_instance_type_str(dc_instance_type type) {
    switch (type) {
        case SENDER:
            return "sender";
        case RECEIVER:
            return "receiver";
        case NONE:
            return "none";
    }
}

union InstanceConcrete {
    struct {
        NormSender *sender;
        const char* data;
        unsigned int data_len;
    } sender;
    struct {
        NormReceiver *receiver;
    } receiver;
};

class NormInstance {
    void update_receiver_rate(double rate);
    void update_sender_rate(double rate);
    uint64_t get_receiver_progress();
    uint64_t get_sender_progress();
    void terminate_receiver();
    void terminate_sender();

public:
    static const int NORM_SENDER_DATA_OBJECT_LEN = 10 * 1024;

    std::mutex lock;
    dc_instance_type type;
    InstanceConcrete concrete;

    NormInstance();
    void recycle();
    void init_receiver(unsigned int transferId, unsigned short port);
    void init_sender(unsigned int transferId, std::string dst, unsigned short port, double rate);

    void update_rate(double rate);
    uint64_t get_progress();
    void terminate();
};

}


#endif //DCCASTHOST_NORMINSTANCE_H
