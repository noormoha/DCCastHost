//
// Created by tang on 4/3/18.
//


#include <normApi.h>
#include <iostream>
#include <sys/epoll.h>
#include "NormSender.h"


using namespace moodycamel;

namespace DCCast {

static void clean(NormSessionHandle session, NormInstanceHandle instance) {
    NormStopSender(session);
    NormDestroySession(session);
    NormDestroyInstance(instance);
}

void* sender_loop(void *args) {
    auto *args_struct = static_cast<sender_loop_args*>(args);

    *args_struct->status = INITIALIZING;

    NormInstanceHandle instance = NormCreateInstance();
    if (instance == NORM_INSTANCE_INVALID) {
        log_error(args_struct->id, "NormCreateInstance(): Failed");
        *args_struct->status = ERROR;

        clean(nullptr, instance);
        pthread_exit(nullptr);
    }
    // The sender has session ID 0
    // TODO THE CORRECT METHOD?
    NormSessionHandle session = NormCreateSession(instance, args_struct->dst.c_str(), args_struct->port, 0);
    if (session == NORM_SESSION_INVALID) {
        log_error(args_struct->id, "NormCreateSession(): Failed");
        *args_struct->status = ERROR;

        clean(session, instance);
        pthread_exit(nullptr);
    }

    NormSetTxOnly(session, true);

    if (!NormSetTxPort(session, args_struct->port)) {
        log_error(args_struct->id, "NormSetTxPort(): Failed");
        *args_struct->status = ERROR;

        clean(session, instance);
        pthread_exit(nullptr);
    }

    if (!NormStartSender(session, 0, NormSender::NORM_SENDER_BUFFER, NormSender::NORM_SEGMENT_SIZE, NormSender::NORM_NUM_DATA, NormSender::NORM_NUM_PARITY)) {
        log_error(args_struct->id, "NormStartSender(): Failed");
        *args_struct->status = ERROR;

        clean(session, instance);
        pthread_exit(nullptr);
    }

    int epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
        log_error(args_struct->id, "epoll_create(): Failed");
        *args_struct->status = ERROR;

        clean(session, instance);
        pthread_exit(nullptr);
    }

    struct epoll_event ep = {EPOLLIN, nullptr};
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, NormGetDescriptor(instance), &ep) != 0) {
        log_error(args_struct->id, "epoll_ctl(): Failed");
        *args_struct->status = ERROR;

        clean(session, instance);
        pthread_exit(nullptr);
    }

    NormSetTxRate(session, args_struct->rate);

    *args_struct->status = PROGRESSING;

    int obj_count = 0;

    for (int i = 0; i < 10; i++) {
        NormObjectHandle obj = NormDataEnqueue(session, args_struct->data, args_struct->data_len);
        if (obj == NORM_OBJECT_INVALID) {
            log_error(args_struct->id, "NormDataEnqueue: Failed");
            break;
        }
        obj_count++;
    }
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
            if (command.type == UPDATE_RATE) {
                NormSetTxRate(session, command.content. updateRate.rate);

                response.type = UPDATE_RATE;
                response.id = args_struct->id;
                response.success = true;

                args_struct->responses->enqueue(response);
            }
            else if(command.type == TERMINATE) {
                running = false;
                *args_struct->status = FINISHING;

                response.type = TERMINATE;
                response.id = args_struct->id;
                response.success = true;

                args_struct->responses->enqueue(response);
            }
            else {
                log_error(args_struct->id, "Command processing: wrong command type");
                DCResponse response{command.type, args_struct->id, false};

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

            clean(session, instance);
            pthread_exit(nullptr);
        }
        if (rv > 0) {
            // There is at least one event
            NormEvent event = {};
            NormGetNextEvent(instance, &event);

#ifdef DEBUG
            std::cout << type_name_map[event.type] << std::endl;
#endif

            if (event.type == NORM_TX_OBJECT_SENT) {
                // Update obj_count and progress
                obj_count--;
                *args_struct->progress += args_struct->data_len;
            }
        }

        // Enqueue data if needed
        while (obj_count < 10) {
            NormObjectHandle obj = NormDataEnqueue(session, args_struct->data, args_struct->data_len);
            if (obj == NORM_OBJECT_INVALID) {
                log_error(args_struct->id, "NormDataEnqueue: Failed");
                break;
            }
            obj_count++;
        }
        // End of the loop
    }
    *args_struct->status = TERMINATED;

    clean(session, instance);

}



DCCast::NormSender::NormSender(unsigned int _id, std::string dst, unsigned short port, uint32_t rate, const char *data, unsigned int data_len) {
    id = _id;
    progress = 0;
    status = TERMINATED;

    // requests and response will be deleted when sender_loop_args is deleted
    requests = new BlockingReaderWriterQueue<DCCommand>(10);
    responses = new BlockingReaderWriterQueue<DCResponse>(10);

    args = new sender_loop_args;
    args->data_len = data_len;
    args->data = data;
    args->status = &status;
    args->progress = &progress;
    args->id = id;
    args->rate = rate;
    args->responses = responses;
    args->requests = requests;
    args->port = port;
    args->dst = dst;

    if (pthread_create(&sender, nullptr, sender_loop, args) != 0) {
        // Safe to set status here since thread is not running
        this->status = ERROR;
        throw DCException("pthread_create(): Failed");
    }

    if (pthread_detach(sender) != 0) {
        // Fatal error that will crash the system
        throw std::runtime_error("pthread_detach(): Failed");
    }
}

uint64_t NormSender::get_progress() {
    return this->progress;
}

dc_status NormSender::get_status() {
    return this->status;
}

unsigned int NormSender::get_id() {
    return this->id;
}

NormSender::~NormSender() {
    delete args;
    delete responses;
    delete requests;
}

}
