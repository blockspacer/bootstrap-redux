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
#include <compiler/data/array.h>
#include <boost/filesystem/path.hpp>

namespace basecode::compiler {

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

    ///////////////////////////////////////////////////////////////////////////

    struct location_t final {
        int32_t line = 0;
        int32_t column = 0;
    };

    struct source_location_t final {
        location_t end{};
        location_t start{};
    };

    ///////////////////////////////////////////////////////////////////////////

    class result_message_t final {
    public:
        enum types {
            info,
            error,
            warning,
            data
        };

        result_message_t(
            std::string_view code,
            std::string_view message,
            const source_location_t& loc = {},
            std::string_view details = "",
            types type = types::info) : _type(type),
                                        _code(code),
                                        _message(message),
                                        _details(details),
                                        _location(loc) {
        }

        [[nodiscard]] inline types type() const {
            return _type;
        }

        [[nodiscard]] inline bool is_error() const {
            return _type == types::error;
        }

        [[nodiscard]] inline std::string_view code() const {
            return _code;
        }

        [[nodiscard]] inline std::string_view details() const {
            return _details;
        }

        [[nodiscard]] inline std::string_view message() const {
            return _message;
        }

        [[nodiscard]] inline const source_location_t& location() const {
            return _location;
        }

    private:
        types _type;
        std::string_view _code{};
        std::string_view _message{};
        std::string_view _details{};
        source_location_t _location{};
    };

    ///////////////////////////////////////////////////////////////////////////

    class result_t final {
    public:
        explicit result_t(
                memory::allocator_t* allocator = memory::default_allocator()) : _messages(allocator) {
            assert(allocator);
        }

        inline void fail() {
            _success = false;
        }

        inline void succeed() {
            _success = true;
        }

        inline void info(
                std::string_view code,
                std::string_view message,
                const source_location_t& loc = {},
                std::string_view details = {}) {
            _messages.add(result_message_t{
                code,
                message,
                loc,
                details,
                result_message_t::types::info});
        }

        inline void error(
                std::string_view code,
                std::string_view message,
                const source_location_t& loc = {},
                std::string_view details = {}) {
            _messages.add(result_message_t{
                code,
                message,
                loc,
                details,
                result_message_t::types::error});
            fail();
        }

        inline void warning(
                std::string_view code,
                std::string_view message,
                const source_location_t& loc = {},
                std::string_view details = {}) {
            _messages.add(result_message_t{
                code,
                message,
                loc,
                details,
                result_message_t::types::warning});
        }

        void remove_code(std::string_view code) {
            for (auto it = _messages.begin(); it != _messages.end(); ++it) {
                if ((*it).code() == code)
                    it = _messages.erase(it);
            }
        }

        [[nodiscard]] inline bool is_failed() const {
            return !_success;
        }

        [[nodiscard]] inline bool has_code(std::string_view code) const {
            for (const auto& msg : _messages)
                if (msg.code() == code)
                    return true;
            return false;
        }

        [[nodiscard]] inline const data::array_t<result_message_t>& messages() const {
            return _messages;
        }

        [[nodiscard]] inline const result_message_t* find_code(std::string_view code) const {
            for (const auto& _message : _messages) {
                if (_message.code() == code)
                    return &_message;
            }
            return nullptr;
        }

    private:
        bool _success = true;
        data::array_t<result_message_t> _messages;
    };

}