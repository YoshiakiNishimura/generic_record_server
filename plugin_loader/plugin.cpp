//#include "descriptor_impl.h"
#include "udf/generic_client_factory.h"
#include "udf/generic_record_impl.h"
#include "udf/plugin_loader.h"
#include "udf/udf_loader.h"
#include <dlfcn.h>
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace plugin::udf;

int main(int argc, char** argv) {
    std::unique_ptr<plugin_loader> loader = std::make_unique<udf_loader>();
    if (argc < 2) {
        std::cerr << "Usage: inspect_plugin <path_to_plugin.so>" << std::endl;
        return 1;
    }
    const char* so_path = argv[1];
    loader->load(so_path);
    auto factory = loader->get_factory();
    if (!factory) {
        std::cerr << "[main] Factory creation failed" << std::endl;
        return 1;
    }
    std::cerr << "[main] Factory created: " << factory << std::endl;
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    auto raw_client = factory->create(channel);
    if (!raw_client) {
        std::cerr << "[main] generic_client creation failed" << std::endl;
        return 1;
    }
    std::unique_ptr<generic_client> client(raw_client);
    try {
        std::cout << "[main] SayHello connect" << std::endl;
        generic_record_impl request;
        request.add_string("hello");

        generic_record_impl response;
        grpc::ClientContext context;

        client->call(context, {0, 0}, request, response);

        if (auto cursor = response.cursor()) {
            if (auto result = cursor->fetch_string()) {
                std::cout << "Greeter received: " << *result << std::endl;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[main] Exception during RPC call: " << ex.what() << std::endl;
    }
    try {
        std::cout << "[main] AddIntOne connect" << std::endl;
        generic_record_impl request;
        request.add_int4(42);

        generic_record_impl response;
        grpc::ClientContext context;

        client->call(context, {0, 1}, request, response);

        if (auto cursor = response.cursor()) {
            if (auto result = cursor->fetch_int4()) {
                std::cout << "Greeter received: " << *result << std::endl;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[main] Exception during RPC call: " << ex.what() << std::endl;
    }
    try {
        std::cout << "[main] SayWolrd connect" << std::endl;
        generic_record_impl request;
        request.add_string("world");

        generic_record_impl response;
        grpc::ClientContext context;

        client->call(context, {0, 2}, request, response);

        if (auto cursor = response.cursor()) {
            if (auto result = cursor->fetch_string()) {
                std::cout << "Greeter received: " << *result << std::endl;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[main] Exception during RPC call: " << ex.what() << std::endl;
    }
    try {
        std::cout << "[main] DecDecimal connect" << std::endl;
        generic_record_impl request;
        request.add_string("Dec");
        request.add_int4(3);
        generic_record_impl response;
        grpc::ClientContext context;

        client->call(context, {0, 3}, request, response);

        if (auto cursor = response.cursor()) {
            if (auto result = cursor->fetch_int8()) {
                std::cout << "Greeter received: " << *result << std::endl;
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[main] Exception during RPC call: " << ex.what() << std::endl;
    }
    loader->unload_all();
    return 0;
}
