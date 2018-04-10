//
// Created by tang on 3/26/18.
//

#include "API.h"

using json = nlohmann::json;
using namespace Pistache;

void DCCastAPI::cleanAll(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res) {
    json response;
    response["error"] = nullptr;
    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));
}
