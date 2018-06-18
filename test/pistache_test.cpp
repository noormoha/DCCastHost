//
// Created by tang on 6/12/18.
//

#include <iostream>
#include <atomic>
#include <pistache/endpoint.h>
#include <signal.h>
#include <pistache/http.h>
#include <pistache/net.h>

using namespace std;
using namespace Pistache;

bool stop(false);

void signalHandler(int signal)
{
    cout << "Signal: " << signal << endl;
    stop = true;
}

class HelloHandler : public Http::Handler
{
public:

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response)
    {
        cout << "Received request " << request.resource() << endl;
        response.send(Http::Code::Ok, "Hello, World");
    }
};

int main()
{
    signal(SIGINT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGKILL, signalHandler);

    cout << "Starting rest server" << endl;
    Address addr(Ipv4::any(), Port(9080));

    auto opts = Http::Endpoint::options().threads(1).flags(Tcp::Options::ReuseAddr);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serveThreaded();
    cout << "Server started" << endl;
    while(!stop)
    {
    }
    server.shutdown();
    cout << "Exiting" << endl;
    return 0;
}