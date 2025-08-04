#pragma once
#include "generic_record.h"
#include <vector>

class generic_record_cursor_impl;

class generic_record_impl : public generic_record {
  public:
    void reset() override;
    void add_int4(std::int32_t value) override;
    void add_int4_null() override;
    void add_int8(std::int64_t value) override;
    void add_int8_null() override;
    void add_string(std::string value) override;
    void add_string_null() override;
    std::unique_ptr<generic_record_cursor> cursor() const override;

  private:
    std::vector<std::optional<std::string>> values_;
};

class generic_record_cursor_impl : public generic_record_cursor {
  public:
    explicit generic_record_cursor_impl(const std::vector<std::optional<std::string>>& values);
    std::optional<std::int32_t> fetch_int4() override;
    std::optional<std::int64_t> fetch_int8() override;
    std::optional<std::string> fetch_string() override;

  private:
    const std::vector<std::optional<std::string>>& values_;
    std::size_t index_ = 0;
};
