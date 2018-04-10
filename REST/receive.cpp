//
// Created by tang on 3/26/18.
//

#include "API.h"
#include "../NORM/NormManager.h"

using json = nlohmann::json;
using namespace Pistache;
using namespace DCCast;

void DCCastAPI::receive(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res) {

    NormManager &instance = NormManager::get_instance();
    json response;

    unsigned int id = 0;
    unsigned short port = 0;
    std::string address;
    try {
        id = req.param(":id").as<unsigned int>();
    }
    catch (std::runtime_error &err){
        response["error"] = "Can not convert transfer id to integer.";
        goto error;
    }
/* Source address is optional now
    if (!req.query().has("src")) {
        response["error"] = "No source address number provided.";
        goto error;
    }
*/
    if (!req.query().has("port")) {
        response["error"] = "No source/destination port number provided.";
        goto error;
    }
    port = std::stoi(req.query().get("port").get());
    if (port < 1000 || port >= 10000) {
        response["error"] = "Port number should be in the range [1000, 9999].";
        goto error;
    }
    if (req.query().has("src")) {
        address = req.query().get("src").get();
    }
    response["error"] = nullptr;
    response["debug"]["id"] = id;
    response["debug"]["src"] = address;
    response["debug"]["port"] = port;

    try {
        instance.receive(id, port);
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

