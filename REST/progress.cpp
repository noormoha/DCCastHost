//
// Created by tang on 3/26/18.
//

#include "API.h"

using json = nlohmann::json;
using namespace Pistache;

void DCCastAPI::progress(const Pistache::Rest::Request& req, Pistache::Http::ResponseWriter res)  {
    json response;
    int id = 0;
    try {
        id = req.param(":id").as<int>();
    }
    catch (std::runtime_error &err){
        response["err"] = "Can not convert transfer id to integer";
        goto error;
    }
    response["err"] = nullptr;
    response["debug"]["id"] = id;
    res.send(Http::Code::Ok, response.dump(), MIME(Application, Json));

    error:
    res.send(Http::Code::Bad_Request, response.dump(), MIME(Application, Json));
}