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

    // Statics
    uint64_t progress;
    dc_status status;
    unsigned int id;

public:

    NormReceiver(unsigned int id, unsigned short port);

    // Configs
    static const int NORM_RECEIVER_BUFFER = 5 * 1024 * 1024;



    // Control Queues
    moodycamel::BlockingReaderWriterQueue<DCCommand> *requests;
    moodycamel::BlockingReaderWriterQueue<DCResponse> *responses;

    // Get Methods
    uint64_t get_progress();
    dc_status get_status();
    unsigned int get_id();
};

}


#endif //DCCASTHOST_NORMRECEIVER_H
