//
// Created by tang on 3/26/18.
//

#include "API.h"
#include "../NORM/NormManager.h"

using json = nlohmann::json;
using namespace Pistache;
using namespace DCCast;

void DCCastAPI::terminate(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res) {

    NormManager &instance = NormManager::get_instance();
    json response;

    unsigned int id = 0;
    try {
        id = req.param(":id").as<unsigned int>();
    }
    catch (std::runtime_error &err){
        response["error"] = "Can not convert transfer id to integer";
        goto error;
    }

    response["error"] = nullptr;
    response["debug"]["id"] = id;

    try {
        instance.terminate(id);
    }
    catch (DCException &e) {
        response["error"] = e.what();
        goto error;
    }

    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));
    std::cout << "Terminate" << std::endl;
    return;

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}
