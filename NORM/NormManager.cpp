//
// Created by tang on 4/6/18.
//

#include "NormManager.h"

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
        auto *new_instacne = new NormInstance();
        instances[id] = new_instacne;
    }
    rv = instances[id];
    guard.unlock();

    return rv;
}

}
