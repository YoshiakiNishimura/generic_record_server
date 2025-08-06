#include "greeter_client.h"
#include "udf/generic_client_factory.h"
using namespace plugin::udf;
class rpc_client_factory : public generic_client_factory {
  public:
    generic_client* create(std::shared_ptr<grpc::Channel> channel) const override {
        return new rpc_client(channel);
    }
};

extern "C" generic_client_factory* tsurugi_create_generic_client_factory(const char* service_name) {
    if (std::string_view(service_name) == "Greeter") { return new rpc_client_factory(); }
    return nullptr;
}

extern "C" void tsurugi_destroy_generic_client_factory(generic_client_factory* ptr) { delete ptr; }

extern "C" void tsurugi_destroy_generic_client(generic_client* ptr) { delete ptr; }
