//
// Created by tang on 4/6/18.
//

#ifndef DCCASTHOST_NORMMANAGER_H
#define DCCASTHOST_NORMMANAGER_H


#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include "NormInstance.h"

namespace DCCast {

class NormManager {
public:
    static NormManager &get_instance();

    void send(unsigned int id, std::string dst, unsigned short port, double rate);
    void receive(unsigned int id, unsigned short port);
    void update_rate(unsigned int id, double rate);
    uint64_t get_progress(unsigned int id);
    void terminate(unsigned int id);
    void active_transfers(std::vector<unsigned int> &senders, std::vector<unsigned int> &receivers);
    void clean_all();

    std::string get_type(unsigned int id);

    //These functions are deleted to prevent copy instance
    NormManager(NormManager &) = delete;
    NormManager(NormManager &&) = delete;
    void operator=(NormManager &) = delete;

private:
    std::shared_timed_mutex lock;
    std::unordered_map<unsigned int, NormInstance*> instances;

    NormInstance* get_norm_instance(unsigned int id);
    NormInstance* get_create_norm_instance(unsigned int id);

    NormManager();
};

}




#endif //DCCASTHOST_NORMMANAGER_H
