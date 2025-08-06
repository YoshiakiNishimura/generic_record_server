#include "udf/generic_record_impl.h"

namespace plugin::udf {

void generic_record_impl::reset() { values_.clear(); }

void generic_record_impl::add_int4(std::int32_t v) { values_.emplace_back(v); }

void generic_record_impl::add_int4_null() { values_.emplace_back(std::nullopt); }

void generic_record_impl::add_int8(std::int64_t v) { values_.emplace_back(v); }

void generic_record_impl::add_int8_null() { values_.emplace_back(std::nullopt); }

void generic_record_impl::add_float(float v) { values_.emplace_back(v); }

void generic_record_impl::add_float_null() { values_.emplace_back(std::nullopt); }

void generic_record_impl::add_double(double v) { values_.emplace_back(v); }

void generic_record_impl::add_double_null() { values_.emplace_back(std::nullopt); }

void generic_record_impl::add_string(std::string value) { values_.emplace_back(std::move(value)); }

void generic_record_impl::add_string_null() { values_.emplace_back(std::nullopt); }

std::unique_ptr<generic_record_cursor> generic_record_impl::cursor() const {
    return std::make_unique<generic_record_cursor_impl>(values_);
}

generic_record_cursor_impl::generic_record_cursor_impl(const std::vector<value_type>& values)
    : values_(values) {}

bool generic_record_cursor_impl::has_next() { return index_ < values_.size(); }

std::optional<std::int32_t> generic_record_cursor_impl::fetch_int4() {
    if (!has_next()) return std::nullopt;
    const auto& opt = values_[index_++];
    if (!opt) return std::nullopt;
    if (auto p = std::get_if<std::int32_t>(&*opt)) return *p;
    return std::nullopt;
}

std::optional<std::int64_t> generic_record_cursor_impl::fetch_int8() {
    if (!has_next()) return std::nullopt;
    const auto& opt = values_[index_++];
    if (!opt) return std::nullopt;
    if (auto p = std::get_if<std::int64_t>(&*opt)) return *p;
    return std::nullopt;
}

std::optional<std::string> generic_record_cursor_impl::fetch_string() {
    if (!has_next()) return std::nullopt;
    const auto& opt = values_[index_++];
    if (!opt) return std::nullopt;
    if (auto p = std::get_if<std::string>(&*opt)) return *p;
    return std::nullopt;
}

std::optional<float> generic_record_cursor_impl::fetch_float() {
    if (!has_next()) return std::nullopt;
    const auto& opt = values_[index_++];
    if (!opt) return std::nullopt;
    if (auto p = std::get_if<float>(&*opt)) return *p;
    return std::nullopt;
}

std::optional<double> generic_record_cursor_impl::fetch_double() {
    if (!has_next()) return std::nullopt;
    const auto& opt = values_[index_++];
    if (!opt) return std::nullopt;
    if (auto p = std::get_if<double>(&*opt)) return *p;
    return std::nullopt;
}

} // namespace plugin::udf
