#include "generic_record_impl.h"

void generic_record_impl::reset() { values_.clear(); }

void generic_record_impl::add_int4(std::int32_t) {}
void generic_record_impl::add_int4_null() {}
void generic_record_impl::add_int8(std::int64_t) {}
void generic_record_impl::add_int8_null() {}

void generic_record_impl::add_string(std::string value) { values_.emplace_back(std::move(value)); }

void generic_record_impl::add_string_null() { values_.emplace_back(std::nullopt); }

std::unique_ptr<generic_record_cursor> generic_record_impl::cursor() const {
    return std::make_unique<generic_record_cursor_impl>(values_);
}

generic_record_cursor_impl::generic_record_cursor_impl(
    const std::vector<std::optional<std::string>>& values)
    : values_(values) {}

std::optional<std::int32_t> generic_record_cursor_impl::fetch_int4() { return std::nullopt; }
std::optional<std::int64_t> generic_record_cursor_impl::fetch_int8() { return std::nullopt; }

std::optional<std::string> generic_record_cursor_impl::fetch_string() {
    if (index_ < values_.size()) { return values_[index_++]; }
    return std::nullopt;
}
