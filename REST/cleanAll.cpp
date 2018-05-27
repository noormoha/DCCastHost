//
// Created by tang on 3/26/18.
//

#include "API.h"
#include "../NORM/NormManager.h"

using json = nlohmann::json;
using namespace Pistache;
using namespace DCCast;

void DCCastAPI::cleanAll(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res) {
    NormManager &instance = NormManager::get_instance();
    json response;

    response["error"] = nullptr;

    try {
        instance.clean_all();
    }
    catch (DCException &e) {
        response["error"] = e.what();
        goto error;
    }
    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));
    std::cout << "CleanAll" << std::endl;
    return;

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}
