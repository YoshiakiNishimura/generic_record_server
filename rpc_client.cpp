#include "rpc_client.h"
#include "sample.grpc.pb.h"
#include "sample.pb.h"

#include <iostream>
#include <stdexcept>

using grpc::ClientContext;
using grpc::Status;
using namespace plugin::udf;
rpc_client::rpc_client(std::shared_ptr<grpc::Channel> channel)
    : greeter_stub_(Greeter::NewStub(channel)), byer_stub_(myapi::Byer::NewStub(channel)) {}

void rpc_client::call(ClientContext& context, function_index_type function_index,
    generic_record& request, generic_record& response) const {
    auto cursor = request.cursor();
    if (!cursor) { throw std::runtime_error("request cursor is null"); }

    response.reset();
    auto fail = [&response]() { response.add_string("RPC failed"); };
    switch (function_index.second) {
        case 0: {
            StringValue req;
            StringValue rep;
            auto arg1 = cursor->fetch_string();
            if (!arg1) { throw std::runtime_error("No input arg1"); }
            req.set_value(*arg1);

            Status status = greeter_stub_->SayHello(&context, req, &rep);
            if (status.ok()) {
                response.add_string(rep.value());
            } else {
                fail();
            }
            break;
        }

        case 1: {
            Int32Value req;
            Int32Value rep;
            auto arg1 = cursor->fetch_int4();
            if (!arg1) { throw std::runtime_error("No input arg1"); }
            req.set_value(*arg1);
            Status status = greeter_stub_->IntAddOne(&context, req, &rep);
            if (status.ok()) {
                response.add_int4(rep.value());
            } else {
                fail();
            }
            break;
        }
        case 2: {
            StringValue req;
            StringValue rep;
            auto arg1 = cursor->fetch_string();
            if (!arg1) { throw std::runtime_error("No input arg1"); }
            req.set_value(*arg1);
            Status status = byer_stub_->SayWorld(&context, req, &rep);
            if (status.ok()) {
                response.add_string(rep.value());
            } else {
                fail();
            }
            break;
        }
        case 3: {
            tsurugidb::udf::value::Decimal req;
            tsurugidb::udf::value::LocalTime rep;
            auto arg1 = cursor->fetch_string();
            if (!arg1) { throw std::runtime_error("No input arg1"); }
            auto arg2 = cursor->fetch_int4();
            if (!arg2) { throw std::runtime_error("No input arg2"); }

            req.set_unscaled_value(*arg1);
            req.set_exponent(*arg2);
            Status status = byer_stub_->DecDecimal(&context, req, &rep);
            if (status.ok()) {
                response.add_int8(rep.nanos());
            } else {
                fail();
            }
            break;
        }

        default: throw std::runtime_error("Unsupported function_index (only 1 or 2 allowed)");
    }
}
