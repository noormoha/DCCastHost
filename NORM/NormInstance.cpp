//
// Created by tang on 4/4/18.
//

#include <pthread.h>
#include <cstring>
#include <sstream>
#include "NormInstance.h"
#include "NormDefinition.h"

namespace DCCast {

DCCast::NormInstance::NormInstance() {
    type = NONE;
    concrete = {};
}

void DCCast::NormInstance::init_receiver(unsigned int transferId, unsigned short port) {
    // Check whether instance is in use
    recycle();

    this->type = RECEIVER;

    this->concrete.receiver.receiver = new NormReceiver(transferId, port);
}

void DCCast::NormInstance::init_sender(unsigned int transferId, std::string dst, unsigned short port, double rate) {
    // Check whether instance is in use
    recycle();

    this->type = SENDER;

    // Construct the fake data to be sent
    auto *data = new char[NORM_SENDER_DATA_OBJECT_LEN];
    for (int i = 0; i + 2 <= NORM_SENDER_DATA_OBJECT_LEN; i += 2) {
        *((unsigned short *) (data + i)) = port;
    }

    this->concrete.sender.data = data;
    this->concrete.sender.data_len = NORM_SENDER_DATA_OBJECT_LEN;
    this->concrete.sender.sender = new NormSender(transferId, dst, port, rate, this->concrete.sender.data,
                                                  this->concrete.sender.data_len);
}

void DCCast::NormInstance::recycle() {
    dc_status status;
    switch (type) {
        case NONE:
            return;
        case RECEIVER:
            if (!this->concrete.receiver.receiver) return;
            status = this->concrete.receiver.receiver->get_status();

            // We can safely forget last receiver thread
            if (status == ERROR || status == TERMINATED) {
                this->concrete = {};
                this->type = NONE;
                return;
            }

            // Receiver is running. Need to terminate it first
            throw DCException("Instance is actively receiving data");
        case SENDER:
            if (!this->concrete.sender.sender) return;
            status = this->concrete.sender.sender->get_status();

            // We can safely forget last sender thread
            if (status == ERROR || status == TERMINATED) {
                this->concrete = {};
                this->type = NONE;
                return;
            }

            // Sender is running. Need to terminate it first
            throw DCException("Instance is actively sending data");
    }

    // Should not come to this place
    throw DCException("Unknown Error at NormInstance::recycle()");
}

void NormInstance::update_rate(double rate) {
    switch (type) {
        case NONE:
            throw DCException("Try to set rate on a uninitialized instance");
        case RECEIVER:
            return update_receiver_rate(rate);
        case SENDER:
            return update_sender_rate(rate);
    }
}

void NormInstance::update_receiver_rate(double rate) {
    NormReceiver *receiver = concrete.receiver.receiver;

    if (receiver->get_status() != PROGRESSING) {
        throw DCException("Receiver is not running");
    }

    DCCommand command = {};
    command.type = UPDATE_RATE;
    command.id = receiver->get_id();
    command.content.updateRate.rate = rate;

    receiver->requests->enqueue(command);
    DCResponse response = {};
    if (!receiver->responses->wait_dequeue_timed(response, std::chrono::seconds(1))) {
        throw DCException("Timeout when waiting for response");
    }

    if (response.id != receiver->get_id() || response.type != UPDATE_RATE) {
        throw std::runtime_error("Inconsistent type/id");
    }

    if (!response.success) {
        throw DCException("Command failed");
    }

    // Success
}

void NormInstance::update_sender_rate(double rate) {
    NormSender *sender = concrete.sender.sender;

    if (sender->get_status() != PROGRESSING) {
        throw DCException("Sender is not running");
    }

    DCCommand command = {};
    command.type = UPDATE_RATE;
    command.id = sender->get_id();
    command.content.updateRate.rate = rate;

    sender->requests->enqueue(command);
    DCResponse response = {};
    if (!sender->responses->wait_dequeue_timed(response, std::chrono::seconds(1))) {
        throw DCException("Timeout when waiting for response");
    }

    if (response.id != sender->get_id() || response.type != UPDATE_RATE) {
        throw std::runtime_error("Inconsistent type/id");
    }

    if (!response.success) {
        throw DCException("Command failed");
    }

    // Success
}

uint64_t NormInstance::get_progress() {
    switch (type) {
        case NONE:
            throw DCException("Try to get progress on a uninitialized instance");
        case RECEIVER:
            return get_receiver_progress();
        case SENDER:
            return get_sender_progress();
    }

}

uint64_t NormInstance::get_receiver_progress() {
    return concrete.receiver.receiver->get_progress();
}

uint64_t NormInstance::get_sender_progress() {
    return concrete.sender.sender->get_progress();
}

void NormInstance::terminate() {
    switch (type) {
        case NONE:
            break;
        case RECEIVER:
            return terminate_receiver();
        case SENDER:
            return terminate_sender();
    }
}

void NormInstance::terminate_receiver() {
    NormReceiver *receiver = concrete.receiver.receiver;

    DC_STATUS status = receiver->get_status();
    if (status == FINISHING || status == TERMINATED || status == ERROR) {
        // Instance is not at running state
        // Do not need to send a terminate command
        return;
    }

    DCCommand command = {};
    command.type = TERMINATE;
    command.id = receiver->get_id();

    receiver->requests->enqueue(command);

    DCResponse response = {};
    if (!receiver->responses->wait_dequeue_timed(response, std::chrono::seconds(1))) {
        throw DCException("Timeout when waiting for response");
    }

    if (response.id != receiver->get_id() || response.type != TERMINATE) {
        throw std::runtime_error("Inconsistent type/id");
    }

    if (!response.success) {
        throw DCException("Command failed");
    }
}

void NormInstance::terminate_sender() {
    NormSender *sender = concrete.sender.sender;

    DC_STATUS status = sender->get_status();
    if (status == FINISHING || status == TERMINATED || status == ERROR) {
        // Instance is not at running state
        // Do not need to send a terminate command
        return;
    }

    DCCommand command = {};
    command.type = TERMINATE;
    command.id = sender->get_id();

    sender->requests->enqueue(command);

    DCResponse response = {};
    if (!sender->responses->wait_dequeue_timed(response, std::chrono::seconds(1))) {
        throw DCException("Timeout when waiting for response");
    }

    if (response.id != sender->get_id() || response.type != TERMINATE) {
        throw std::runtime_error("Inconsistent type/id");
    }

    if (!response.success) {
        throw DCException("Command failed");
    }
}

}


