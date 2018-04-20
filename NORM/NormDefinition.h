//
// Created by tang on 4/3/18.
//

#ifndef DCCASTHOST_NORMDEFINITION_H
#define DCCASTHOST_NORMDEFINITION_H

#include <cstdint>
#include <iostream>


namespace DCCast {

    typedef enum DC_STATUS {
        INITIALIZING,
        PROGRESSING,
        FINISHING,
        ERROR,
        TERMINATED
    } dc_status;

    typedef enum DC_COMMAND_TYPE {
        TERMINATE,
        UPDATE_RATE
    } dc_command_type;

    union DCCommandContent {
        struct {
            double rate;
        } updateRate;
    };

    struct DCCommand {
        dc_command_type type;
        unsigned int id;
        DCCommandContent content;
    };

union DCResponseContent {

};

    struct DCResponse {
        dc_command_type type;
        unsigned int id;
        bool success;
        DCResponseContent content;
    };


static void log_error(unsigned int id, const std::string error_message) {
    std::cerr << id << ' ' << error_message << std::endl;
}

static std::string type_name_map[] = {
        "NORM_EVENT_INVALID",
        "NORM_TX_QUEUE_VACANCY",
        "NORM_TX_QUEUE_EMPTY",
        "NORM_TX_FLUSH_COMPLETED",
        "NORM_TX_WATERMARK_COMPLETED",
        "NORM_TX_CMD_SENT",
        "NORM_TX_OBJECT_SENT",
        "NORM_TX_OBJECT_PURGED",
        "NORM_TX_RATE_CHANGED",
        "NORM_LOCAL_SENDER_CLOSED",
        "NORM_REMOTE_SENDER_NEW",
        "NORM_REMOTE_SENDER_RESET",     // remote sender instanceId or FEC params changed
        "NORM_REMOTE_SENDER_ADDRESS",   // remote sender src addr and/or port changed
        "NORM_REMOTE_SENDER_ACTIVE",
        "NORM_REMOTE_SENDER_INACTIVE",
        "NORM_REMOTE_SENDER_PURGED",    // not yet implemented
        "NORM_RX_CMD_NEW",
        "NORM_RX_OBJECT_NEW",
        "NORM_RX_OBJECT_INFO",
        "NORM_RX_OBJECT_UPDATED",
        "NORM_RX_OBJECT_COMPLETED",
        "NORM_RX_OBJECT_ABORTED",
        "NORM_GRTT_UPDATED",
        "NORM_CC_ACTIVE",
        "NORM_CC_INACTIVE",
        "NORM_ACKING_NODE_NEW",
        "NORM_SEND_ERROR",
        "NORM_USER_TIMEOUT"
};

class DCException : std::runtime_error {
    std::string error;
public:
    const char* what() const noexcept override;
    explicit DCException(std::string error);
};
}

#endif //DCCASTHOST_NORMDEFINITION_H
