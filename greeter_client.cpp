#include "greeter_client.h"
#include "greeter.grpc.pb.h"
#include "greeter.pb.h"

#include <iostream>
#include <stdexcept>

using grpc::ClientContext;
using grpc::Status;
using namespace plugin::udf;
greeter_client::greeter_client(std::shared_ptr<grpc::Channel> channel)
    : stub_(Greeter::NewStub(channel)) {}

void greeter_client::call(ClientContext& context, function_index_type function_index,
    generic_record& request, generic_record& response) const {
    if (function_index != 0) {
        throw std::runtime_error("Unsupported function_index (only 0 allowed)");
    }

    auto cursor = request.cursor();
    if (!cursor) { throw std::runtime_error("request cursor is null"); }

    auto maybe_user = cursor->fetch_string();
    if (!maybe_user) { throw std::runtime_error("No input string provided"); }

    StringValue req;
    req.set_value(*maybe_user);

    StringValue rep;

    Status status = stub_->SayHello(&context, req, &rep);

    response.reset();
    if (status.ok()) {
        response.add_string(rep.value());
    } else {
        std::cerr << "RPC failed: " << status.error_message() << std::endl;
        response.add_string("RPC failed");
    }
}
