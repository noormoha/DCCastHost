//
// Created by tang on 3/26/18.
//

#include "API.h"
#include "../NORM/NormManager.h"

using json = nlohmann::json;
using namespace Pistache;
using namespace DCCast;

void DCCastAPI::send(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res) {

    NormManager &instance = NormManager::get_instance();
    json response;

    unsigned int id = 0;
    unsigned short port = 0;
    uint32_t rate = 0;
    std::string address;
    try {
        id = req.param(":id").as<unsigned int>();
    }
    catch (std::runtime_error &err){
        response["error"] = "Can not convert transfer id to integer.";
        goto error;
    }

    if (!req.query().has("dst")) {
        response["error"] = "No destination address provided.";
        goto error;
    }
    if (!req.query().has("port")) {
        response["error"] = "No source/destination port number provided.";
        goto error;
    }
    if (!req.query().has("rate")) {
        response["error"] = "No rate provided.";
        goto error;
    }

    port = std::stoi(req.query().get("port").get());
    if (port < 1000 || port >= 10000) {
        response["error"] = "Port number should be in the range [1000, 9999].";
        goto error;
    }
    rate = std::stoi(req.query().get("rate").get());
    if (rate <= 0) {
        response["error"] = "Rate should be larger than 0.";
        goto error;
    }
    address = req.query().get("dst").get();

    response["error"] = nullptr;
    response["debug"]["id"] = id;
    response["debug"]["dst"] = address;
    response["debug"]["port"] = port;
    response["debug"]["rate"] = rate;

    try {
        instance.send(id, address, port, rate);
    }
    catch (DCException &e) {
        response["error"] = e.what();
        goto error;
    }

    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));
    return;

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}