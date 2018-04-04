//
// Created by tang on 4/3/18.
//

#ifndef DCCASTHOST_NORMSENDER_H
#define DCCASTHOST_NORMSENDER_H

#include "../queue/readerwriterqueue.h"
#include "NormDefinition.h"

namespace DCCast {


class NormSender {
    // Receiver Thread
    pthread_t sender;

public:

    NormSender(unsigned int id, std::string dst, unsigned short port, uint32_t rate, const char *data, unsigned int data_len);

    // Configs
    static const int NORM_SENDER_BUFFER = 20 * 1024 * 1024;
    static const int NORM_SEGMENT_SIZE = 1300;
    static const int NORM_NUM_DATA = 64;
    static const int NORM_NUM_PARITY = 16;

    // Statics
    uint64_t progress;
    dc_status status;

    // Control Queues
    moodycamel::BlockingReaderWriterQueue<DCCommand> *requests;
    moodycamel::BlockingReaderWriterQueue<DCResponse> *responses;
};

}

#endif //DCCASTHOST_NORMSENDER_H
