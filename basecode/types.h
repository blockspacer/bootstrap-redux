// ----------------------------------------------------------------------------
// ____                               _
// |  _\                             | |
// | |_)| __ _ ___  ___  ___ ___   __| | ___ TM
// |  _< / _` / __|/ _ \/ __/ _ \ / _` |/ _ \
// | |_)| (_| \__ \  __/ (_| (_) | (_| |  __/
// |____/\__,_|___/\___|\___\___/ \__,_|\___|
//
//       C O M P I L E R  P R O J E C T
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// This software source file is licensed under the terms of MIT license.
// For details, please read the LICENSE file.
//
// ----------------------------------------------------------------------------

#pragma once

#include <stack>
#include <string>
#include <vector>
#include <cstdint>
#include <utility>
#include <entt/entt.hpp>
#include <boost/uuid/uuid.hpp>
#include <basecode/data/array.h>
#include <boost/filesystem/path.hpp>

namespace basecode {

    // filesystem type aliases
    using path_t = boost::filesystem::path;

    // string type aliases
    using string_t = data::array_t<char>;
    using string_list_t = data::array_t<std::string>;
    using slice_list_t = data::array_t<std::string_view>;

    // time type aliases
    using time_point_t = std::chrono::system_clock::time_point;
    using monotonic_time_point_t = std::chrono::steady_clock::time_point;

    // bytes type aliases
    using byte_buffer_t = data::array_t<uint8_t>;

    // uuid type aliases
    using uuid_t = boost::uuids::uuid;

    // entity aliases
    using entity_t = entt::entity;
    using entity_list_t = data::array_t<entt::entity>;

}