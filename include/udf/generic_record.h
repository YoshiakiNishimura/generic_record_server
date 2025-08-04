#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace plugin::udf {
class generic_record_cursor {
  public:
    virtual ~generic_record_cursor() = default;

    virtual std::optional<std::int32_t> fetch_int4()  = 0;
    virtual std::optional<std::int64_t> fetch_int8()  = 0;
    virtual std::optional<std::string> fetch_string() = 0;
};class generic_record {
  public:
    virtual ~generic_record() = default;

    virtual void reset() = 0;

    virtual void add_int4(std::int32_t value) = 0;
    virtual void add_int4_null()              = 0;

    virtual void add_int8(std::int64_t value) = 0;
    virtual void add_int8_null()              = 0;

    virtual void add_string(std::string value) = 0;
    virtual void add_string_null()             = 0;

    virtual std::unique_ptr<generic_record_cursor> cursor() const = 0;
};

} // namespace plugin::udf
