//
// Created by tang on 4/6/18.
//

#include "NormManager.h"
#include "NormInstance.h"

namespace DCCast {

NormManager &NormManager::get_instance() {
    static NormManager instance;
    return instance;
}

NormManager::NormManager() { }

void NormManager::send(unsigned int id, std::string dst, unsigned short port, uint32_t rate) {
    // Get instance first
    NormInstance *cur_instance = get_create_norm_instance(id);

    std::unique_lock<std::mutex> instance_guard(cur_instance->lock);
    cur_instance->init_sender(id, dst, port, rate);
    instance_guard.unlock();
}

void NormManager::receive(unsigned int id, unsigned short port) {
    // Get Instance First
    NormInstance *cur_instance = get_create_norm_instance(id);

    std::unique_lock<std::mutex> instance_guard(cur_instance->lock);
    cur_instance->init_receiver(id, port);
    instance_guard.unlock();
}

void NormManager::update_rate(unsigned int id, uint32_t rate) {
    // Get instance first
    NormInstance *cur_instance = get_norm_instance(id);

    if (!cur_instance) {
        throw DCException("Can not find provided transfer id");
    }

    std::unique_lock<std::mutex> instance_guard(cur_instance->lock);
    cur_instance->update_rate(rate);

}

uint64_t NormManager::get_progress(unsigned int id) {
    // Get instance first
    NormInstance *cur_instance = get_norm_instance(id);

    if (!cur_instance) {
        throw DCException("Can not find provided transfer id");
    }

    std::unique_lock<std::mutex> instance_guard(cur_instance->lock);
    return cur_instance->get_progress();
}

NormInstance *NormManager::get_norm_instance(unsigned int id) {
    NormInstance *rv = nullptr;
    std::shared_lock<std::shared_timed_mutex> guard(lock);
    if (instances.count(id)) {
        rv = instances[id];
    }
    guard.unlock();

    return rv;
}

NormInstance *NormManager::get_create_norm_instance(unsigned int id) {
    NormInstance *rv = nullptr;
    std::unique_lock<std::shared_timed_mutex> guard(lock);
    if (!instances.count(id)) {
        // Create a instance if not exists
        auto *new_instance = new NormInstance();
        instances[id] = new_instance;
    }
    rv = instances[id];
    guard.unlock();

    return rv;
}

void NormManager::terminate(unsigned int id) {
    // Get instance first
    NormInstance *cur_instance = get_norm_instance(id);

    if (!cur_instance) {
        throw DCException("Can not find provided transfer id");
    }

    std::unique_lock<std::mutex> instance_guard(cur_instance->lock);
    return cur_instance->terminate();
}

void NormManager::active_transfers(std::vector<unsigned int> &senders, std::vector<unsigned int> &receivers) {
    // Lock instances
    std::shared_lock<std::shared_timed_mutex> guard(lock);

    // Traverse all instances
    for (auto &ins_pair : instances) {
        NormInstance *ins = ins_pair.second;
        switch (ins->type) {
            case RECEIVER:
                if (ins->concrete.receiver.receiver->get_status() == PROGRESSING) {
                    receivers.push_back(ins->concrete.receiver.receiver->get_id());
                }
                break;
            case SENDER:
                if (ins->concrete.sender.sender->get_status() == PROGRESSING) {
                    senders.push_back(ins->concrete.sender.sender->get_id());
                }
                break;
            default:
                break;
        }
    }
}

std::string NormManager::get_type(unsigned int id) {
    NormInstance *instance = get_norm_instance(id);
    return dc_instance_type_str(instance->type);
}


}
