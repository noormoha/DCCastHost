//
// Created by tang on 3/26/18.
//

#include "API.h"
#include "../NORM/NormManager.h"

using json = nlohmann::json;
using namespace Pistache;
using namespace DCCast;

void DCCastAPI::activeTransfer(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res) {

    NormManager &instance = NormManager::get_instance();
    json response;

    response["error"] = nullptr;
    response["senders"] = json::array();
    response["receivers"] = json::array();

    std::vector<unsigned int> receivers, senders;

    try {
        instance.active_transfers(senders, receivers);
    }
    catch (DCException &e) {
        response["error"] = e.what();
        goto error;
    }
    response["senders"] = senders;
    response["receivers"] = receivers;

    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));
    std::cout << "ActiveTransfer" << std::endl;
    return;

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}