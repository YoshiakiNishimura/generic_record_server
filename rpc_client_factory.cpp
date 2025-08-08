#include "rpc_client.h"
#include "udf/generic_client_factory.h"
using namespace plugin::udf;
class rpc_client_factory : public generic_client_factory {
  public:
    generic_client* create(std::shared_ptr<grpc::Channel> channel) const override {
        return new rpc_client(channel);
    }
};

extern "C" generic_client_factory* tsurugi_create_generic_client_factory(const char* service_name) {
    std::cerr << "[tsurugi_create_generic_client_factory] Creating factory for service: "
              << service_name << std::endl;
    if (std::string_view(service_name) == "Greeter") {
        std::cerr << "[tsurugi_create_generic_client_factory] Returning rpc_client_factory"
                  << std::endl;
        return new rpc_client_factory();
    }
    std::cerr << "[tsurugi_create_generic_client_factory] No matching factory for service: "
              << service_name << std::endl;

    return nullptr;
}

extern "C" void tsurugi_destroy_generic_client_factory(generic_client_factory* ptr) { delete ptr; }

extern "C" void tsurugi_destroy_generic_client(generic_client* ptr) { delete ptr; }
