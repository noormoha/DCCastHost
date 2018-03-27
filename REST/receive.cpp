//
// Created by tang on 3/26/18.
//

#include "API.h"

using json = nlohmann::json;
using namespace Pistache;

void DCCastAPI::receive(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res) {
    json response;
    int id = 0;
    int port = 0;
    std::string address;
    try {
        id = req.param(":id").as<int>();
    }
    catch (std::runtime_error &err){
        response["err"] = "Can not convert transfer id to integer.";
        goto error;
    }

    if (!req.query().has("src")) {
        response["err"] = "No source address number provided.";
        goto error;
    }
    if (!req.query().has("port")) {
        response["err"] = "No source/destination port number provided.";
        goto error;
    }
    port = std::stoi(req.query().get("port").get());
    if (port < 1000 || port >= 10000) {
        response["err"] = "Port number should be in the range [1000, 9999].";
        goto error;
    }
    address = req.query().get("src").get();
    response["err"] = nullptr;
    response["debug"]["id"] = id;
    response["debug"]["src"] = address;
    response["debug"]["port"] = port;
    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}

