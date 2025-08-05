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
    auto cursor = request.cursor();
    if (!cursor) { throw std::runtime_error("request cursor is null"); }

    response.reset();
    auto fail = [&response]() { response.add_string("RPC failed"); };
    switch (function_index.second) {
        case 0: { // SayHello expects 0 string
            auto maybe_user = cursor->fetch_string();
            if (!maybe_user) { throw std::runtime_error("No input string provided"); }

            StringValue req;
            req.set_value(*maybe_user);
            StringValue rep;

            Status status = stub_->SayHello(&context, req, &rep);
            if (status.ok()) {
                response.add_string(rep.value());
            } else {
                fail();
            }
            break;
        }

        case 1: { // IntAddInt expects 1 int32 inputs
            auto maybe_user = cursor->fetch_int4();
            Int32Value req;
            req.set_value(*maybe_user);
            Int32Value rep;
            Status status = stub_->IntAddOne(&context, req, &rep);
            if (status.ok()) {
                response.add_int4(rep.value());
            } else {
                fail();
            }
            break;
        }

        default: throw std::runtime_error("Unsupported function_index (only 1 or 2 allowed)");
    }
}
