//
// Created by tang on 4/3/18.
//

#ifndef DCCASTHOST_NORMRECEIVER_H
#define DCCASTHOST_NORMRECEIVER_H

#include "../queue/readerwriterqueue.h"
#include "NormDefinition.h"

namespace DCCast {

class NormReceiver {
    // Receiver Thread
    pthread_t receiver;
public:

    // Configs
    static const int NORM_RECEIVER_BUFFER = 5 * 1024 * 1024;

    // Statics
    uint64_t progress;
    dc_status status;

    // Control Queues
    moodycamel::BlockingReaderWriterQueue<DCCommand> *requests;
    moodycamel::BlockingReaderWriterQueue<DCResponse> *responses;

    NormReceiver(unsigned int id, std::string src, unsigned short port);
};

}


#endif //DCCASTHOST_NORMRECEIVER_H
