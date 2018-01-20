#include <iostream>

#include "./includes/API.hpp"


#include "pistache/endpoint.h"
using namespace Pistache;

struct HelloHandler : public Http::Handler {
    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
        response.send(Http::Code::Ok, "Hello, World!");
    }
};

int main()
{
    // API initialization
    std::cout << "Initalizing API...";
    bool resultAPI = API::initialize();
    if (resultAPI) {
        std::cout << "done" << std::endl;
    } else {
        std::cout << "failed" << std::endl;
    }

    Http::listenAndServe<HelloHandler>("*:9080");

    return 0;
}
