#include <iostream>
#include <memory>
#include <string>

#include "greeter.grpc.pb.h"
#include "greeter.pb.h"
#include <grpcpp/grpcpp.h>

using ::Greeter;
using ::StringValue;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class GreeterServiceImpl final : public Greeter::Service {
    Status SayHello(
        ServerContext* context, const StringValue* request, StringValue* reply) override {
        std::string prefix("Hello ");
        reply->set_value(prefix + request->value());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
