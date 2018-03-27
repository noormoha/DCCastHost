//
// Created by tang on 3/26/18.
//

#include "API.h"

using json = nlohmann::json;
using namespace Pistache;

void DCCastAPI::send(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res) {
    json response;
    int id = 0;
    int port = 0;
    int rate = 0;
    long long volume = 0;
    std::string address;
    try {
        id = req.param(":id").as<int>();
    }
    catch (std::runtime_error &err){
        response["err"] = "Can not convert transfer id to integer.";
        goto error;
    }

    if (!req.query().has("dst")) {
        response["err"] = "No destination address provided.";
        goto error;
    }
    if (!req.query().has("port")) {
        response["err"] = "No source/destination port number provided.";
        goto error;
    }
    if (!req.query().has("volume")) {
        response["err"] = "No volume provided.";
        goto error;
    }
    if (!req.query().has("rate")) {
        response["err"] = "No rate provided.";
        goto error;
    }

    port = std::stoi(req.query().get("port").get());
    if (port < 1000 || port >= 10000) {
        response["err"] = "Port number should be in the range [1000, 9999].";
        goto error;
    }
    volume = std::stoll(req.query().get("volume").get());
    if (volume <= 0) {
        response["err"] = "Volume should be larger than 0.";
        goto error;
    }
    rate = std::stoi(req.query().get("rate").get());
    if (rate <= 0) {
        response["err"] = "Rate should be larger than 0.";
        goto error;
    }
    address = req.query().get("dst").get();

    response["err"] = nullptr;
    response["debug"]["id"] = id;
    response["debug"]["dst"] = address;
    response["debug"]["port"] = port;
    response["debug"]["volume"] = volume;
    response["debug"]["rate"] = rate;
    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}