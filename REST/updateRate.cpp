//
// Created by tang on 3/26/18.
//

#include "API.h"
#include "../NORM/NormManager.h"

using json = nlohmann::json;
using namespace Pistache;
using namespace DCCast;

void DCCastAPI::updateRate(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res) {

    NormManager &instance = NormManager::get_instance();
    json response;

    unsigned int id = 0;
    double rate = 0;
    try {
        id = req.param(":id").as<unsigned int>();
    }
    catch (std::runtime_error &err){
        response["error"] = "Can not convert transfer id to integer.";
        goto error;
    }

    if (!req.query().has("rate")) {
        response["error"] = "No rate provided.";
        goto error;
    }
    rate = std::stod(req.query().get("rate").get());
    if (rate <= 0) {
        response["error"] = "Rate should be a number larger than 0.";
        goto error;
    }
    response["err"] = nullptr;
    response["debug"]["id"] = id;
    response["debug"]["rate"] = rate;

    try {
        instance.update_rate(id, rate);
    }
    catch (DCException &e) {
        response["error"] = e.what();
        goto error;
    }

    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));
    std::cout << "UpdateRate" << std::endl;
    return;

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}
