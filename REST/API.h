//
// Created by tang on 3/26/18.
//

#ifndef DCCASTHOST_API_H
#define DCCASTHOST_API_H

#include <pistache/router.h>
#include "../json.hpp"

namespace DCCastAPI {
    void progress(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res);
    void cleanAll(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res);
    void activeTransfer(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res);
    void updateRate(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res);
    void terminate(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res);
    void receive(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res);
    void send(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res);
}

#endif //DCCASTHOST_API_H
