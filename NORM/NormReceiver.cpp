//
// Created by tang on 4/3/18.
//

#include <normApi.h>
#include <sys/epoll.h>
#include "NormReceiver.h"

using namespace moodycamel;

namespace DCCast {

struct receiver_loop_args {
    dc_status *status;
    uint64_t *progress;
    unsigned int id;
    BlockingReaderWriterQueue<DCCommand> *requests;
    BlockingReaderWriterQueue<DCResponse> *responses;
    unsigned short port;
    std::string src;
};

void* receiver_loop(void *args) {
    auto *args_struct = static_cast<receiver_loop_args*>(args);

    *args_struct->status = INITIALIZING;

    NormInstanceHandle instance = NormCreateInstance();
    if (instance == NORM_INSTANCE_INVALID) {
        log_error(args_struct->id, "NormCreateInstance(): Failed");
        *args_struct->status = ERROR;
        pthread_exit(nullptr);
    }
    // All receivers have session ID 1
    // TODO IS THIS THE CORRECT METHOD?
    NormSessionHandle session = NormCreateSession(instance, "0.0.0.0", args_struct->port, 1);
    if (session == NORM_SESSION_INVALID) {
        log_error(args_struct->id, "NormCreateSession(): Failed");
        *args_struct->status = ERROR;
        pthread_exit(nullptr);
    }

    NormSetDefaultUnicastNack(session, true);

    if (!NormStartReceiver(session, NormReceiver::NORM_RECEIVER_BUFFER)) {
        log_error(args_struct->id, "NormStartReceiver(): Failed");
        *args_struct->status = ERROR;
        pthread_exit(nullptr);
    }

    int epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
        log_error(args_struct->id, "epoll_create(): Failed");
        *args_struct->status = ERROR;
        pthread_exit(nullptr);
    }

    struct epoll_event ep = {EPOLLIN, nullptr};
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, NormGetDescriptor(instance), &ep) != 0) {
        log_error(args_struct->id, "epoll_ctl(): Failed");
        *args_struct->status = ERROR;
        pthread_exit(nullptr);
    }

    *args_struct->status = PROGRESSING;

    // Loop begin
    bool running = true;
    while (running) {
        // Process all commands
        DCCommand command = {};
        DCResponse response = {};
        while (args_struct->requests->try_dequeue(command)) {
            if (!running) break;

            if (command.id != args_struct->id) {
                log_error(args_struct->id, "Wrong id in command");
                continue;
            }

            // Process by type
            if(command.type == TERMINATE) {
                running = false;
                *args_struct->status = FINISHING;

                response.type = TERMINATE;
                response.id = args_struct->id;
                response.success = true;

                args_struct->responses->enqueue(response);
            }
            else {
                log_error(args_struct->id, "Command processing: wrong command type");
                response.type = command.type;
                response.id = args_struct->id;
                response.success = false;

                args_struct->responses->enqueue(response);
            }
        }

        // Process NORM events
        ep = {};
        // BLocking call
        int rv = epoll_wait(epoll_fd, &ep, 1, 200);
        if (rv < 0) {
            log_error(args_struct->id, "epoll_wait(): Failed");
            *args_struct->status = ERROR;
            pthread_exit(nullptr);
        }
        if (rv > 0) {
            // There is at least one event
            NormEvent event = {};
            NormGetNextEvent(instance, &event);
            std::cout << type_name_map[event.type] << std::endl;

            if (event.type == NORM_RX_OBJECT_COMPLETED) {
                NormObjectHandle obj = event.object;
                if (obj == NORM_OBJECT_INVALID) {
                    log_error(args_struct->id, "Receive empty object. What's happening?");
                    *args_struct->status = ERROR;
                    pthread_exit(nullptr);
                }

                long long size = NormObjectGetSize(obj);
                if (size < 0) {
                    log_error(args_struct->id, "Negative length object");
                    *args_struct->status = ERROR;
                    pthread_exit(nullptr);
                }

                *args_struct->progress += size;
            }
        }
        //End of the loop
    }
    *args_struct->status = TERMINATED;
}

DCCast::NormReceiver::NormReceiver(unsigned int _id, unsigned short port) {
    id = _id;

    requests = new BlockingReaderWriterQueue<DCCommand>(10);
    responses = new BlockingReaderWriterQueue<DCResponse>(10);

    auto *args = new receiver_loop_args;
    args->status = &status;
    args->progress = &progress;
    args->id = id;
    args->responses = responses;
    args->requests = requests;
    args->port = port;

    if (pthread_create(&receiver, nullptr, receiver_loop, args) != 0) {
        log_error(id, "pthread_create(): Failed");
        status = ERROR;
        return;
    }

    if (pthread_detach(receiver) != 0) {
        log_error(id, "pthread_detach(): Failed");
        status = ERROR;
        return;
    }
}

uint64_t NormReceiver::get_progress() {
    return this->progress;
}

dc_status NormReceiver::get_status() {
    return this->status;
}

unsigned int NormReceiver::get_id() {
    return this->id;
}

}
