#include "generic_client_factory.h"
#include "generic_record_impl.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>

int main() {
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());

    generic_client_factory* factory = tsurugi_create_generic_client_factory("Greeter");
    if (!factory) {
        std::cerr << "Factory creation failed\n";
        return 1;
    }

    std::unique_ptr<generic_client> client(factory->create(channel));
    if (!client) {
        std::cerr << "Client creation failed\n";
        tsurugi_destroy_generic_client_factory(factory);
        return 1;
    }

    generic_record_impl request;
    request.add_string("world");

    generic_record_impl response;
    grpc::ClientContext context;

    client->call(context, 0, request, response);

    auto cursor = response.cursor();
    if (cursor) {
        auto result = cursor->fetch_string();
        if (result) { std::cout << "Greeter received: " << *result << std::endl; }
    }

    tsurugi_destroy_generic_client(client.release());
    tsurugi_destroy_generic_client_factory(factory);

    return 0;
}
