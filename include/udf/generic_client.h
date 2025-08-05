#pragma once

#include "generic_record.h"
#include <cstddef>
#include <grpcpp/client_context.h>
#include <utility>
namespace plugin::udf {
class generic_client {
  public:
    using function_index_type = std::pair<int, int>;
    virtual ~generic_client() = default;

    virtual void call(grpc::ClientContext& context, function_index_type function_index,
        generic_record& request, generic_record& response) const = 0;
};
} // namespace plugin::udf