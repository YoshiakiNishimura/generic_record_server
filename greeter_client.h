#pragma once

#include "greeter.grpc.pb.h"
#include "udf/generic_client.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

using myapi::Greeter;
using myapi::Int32Value;
using myapi::StringValue;

using namespace plugin::udf;
class greeter_client : public generic_client {
  public:
    explicit greeter_client(std::shared_ptr<grpc::Channel> channel);

    void call(grpc::ClientContext& context, function_index_type function_index,
        generic_record& request, generic_record& response) const override;

  private:
    std::unique_ptr<Greeter::Stub> stub_;
};
