/*
 * Copyright 2018-2025 Project Tsurugi.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "descriptor_impl.h"
#include <vector>
#include <string_view>
using namespace plugin::udf;

class plugin_api_impl : public plugin_api {
  public:
    plugin_api_impl() {}

    const std::vector<package_descriptor*>& packages() const noexcept override { return packages_; }
  private:
    std::vector<package_descriptor*> packages_;
};

extern "C" plugin_api* create_plugin_api() {
    return new plugin_api_impl();
}

