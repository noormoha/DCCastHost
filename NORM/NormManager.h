//
// Created by tang on 4/6/18.
//

#ifndef DCCASTHOST_NORMMANAGER_H
#define DCCASTHOST_NORMMANAGER_H


#include <zconf.h>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include "NormInstance.h"

namespace DCCast {

class NormManager {
public:
    static NormManager &get_instance();

    void send(unsigned int id, std::string dst, unsigned short port, uint32_t rate);
    void receive(unsigned int id, unsigned short port);
    void update_rate(unsigned int id, uint32_t rate);
    uint64_t get_progress(unsigned int id);
    void terminate(unsigned int id);

    //These functions are deleted to prevent copy instance
    NormManager(NormManager &) = delete;
    NormManager(NormManager &&) = delete;
    void operator=(NormManager &) = delete;

private:
    std::shared_timed_mutex lock;
    std::unordered_map<int, NormInstance*> instances;

    NormInstance* get_norm_instance(unsigned int id);
    NormInstance* get_create_norm_instance(unsigned int id);

    NormManager();
};

}




#endif //DCCASTHOST_NORMMANAGER_H
