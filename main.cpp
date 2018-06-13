//
// Created by tang on 3/26/18.
//

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "REST/API.h"

using namespace Pistache;

int main() {
    Address address = Address("*:9080");
    Http::Endpoint server(address);

    auto opt = Http::Endpoint::options().threads(1).flags(Tcp::Options::ReuseAddr);
    server.init(opt);

    Rest::Router router;
    Rest::Routes::Post(router, "/cleanAll", Rest::Routes::bind(DCCastAPI::cleanAll));
    Rest::Routes::Get(router, "/progress/:id", Rest::Routes::bind(DCCastAPI::progress));
    Rest::Routes::Get(router, "/activeTransfer", Rest::Routes::bind(DCCastAPI::activeTransfer));
    Rest::Routes::Post(router, "/updateRate/:id", Rest::Routes::bind(DCCastAPI::updateRate));
    Rest::Routes::Post(router, "/terminate/:id", Rest::Routes::bind(DCCastAPI::terminate));
    Rest::Routes::Post(router, "/receive/:id", Rest::Routes::bind(DCCastAPI::receive));
    Rest::Routes::Post(router, "/send/:id", Rest::Routes::bind(DCCastAPI::send));

    server.setHandler(router.handler());
    std::cout << "Server start" << std::endl;
    server.serve();
}