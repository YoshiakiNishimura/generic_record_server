#pragma once

#include "generic_client.h"
#include <grpcpp/channel.h>
#include <memory>
#include <string_view>
namespace plugin::udf {
class generic_client_factory {
  public:
    virtual ~generic_client_factory()                                            = default;
    virtual generic_client* create(std::shared_ptr<grpc::Channel> channel) const = 0;
};

extern "C" {
generic_client_factory* tsurugi_create_generic_client_factory(const char* service_name);
void tsurugi_destroy_generic_client_factory(generic_client_factory* ptr);
void tsurugi_destroy_generic_client(generic_client* ptr);
}

} // namespace plugin::udf
