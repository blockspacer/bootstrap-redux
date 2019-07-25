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

#include <string>
#include <vector>
#include <cstdint>
#include <utility>
#include <boost/uuid/uuid.hpp>
#include <boost/filesystem/path.hpp>

namespace basecode::compiler {

    // filesystem type aliases
    using path_t = boost::filesystem::path;

    // string type aliases
    using string_list_t = std::vector<std::string>;

    // time type aliases
    using time_point_t = std::chrono::system_clock::time_point;
    using monotonic_time_point_t = std::chrono::steady_clock::time_point;

    // bytes type aliases
    using byte_buffer_t = std::vector<uint8_t>;

    // uuid type aliases
    using uuid_t = boost::uuids::uuid;

    ///////////////////////////////////////////////////////////////////////////

    struct location_t {
        uint16_t line = 0;
        uint16_t column = 0;
    };

    struct source_location_t {
        location_t start{};
        location_t end{};
    };

    ///////////////////////////////////////////////////////////////////////////

    class result_message_t {
    public:
        using list_t = std::vector<result_message_t>;

        enum types {
            info,
            error,
            warning,
            data
        };

        result_message_t(
            std::string code,
            std::string message,
            const source_location_t& loc = {},
            std::string details = "",
            types type = types::info) : _type(type),
                                        _code(std::move(code)),
                                        _message(std::move(message)),
                                        _details(std::move(details)),
                                        _location(loc) {
        }

        [[nodiscard]] inline types type() const {
            return _type;
        }

        [[nodiscard]] inline bool is_error() const {
            return _type == types::error;
        }

        [[nodiscard]] inline const std::string& code() const {
            return _code;
        }

        [[nodiscard]] inline const std::string& details() const {
            return _details;
        }

        [[nodiscard]] inline const std::string& message() const {
            return _message;
        }

        [[nodiscard]] inline const source_location_t& location() const {
            return _location;
        }

    private:
        types _type;
        std::string _code{};
        std::string _message {};
        std::string _details {};
        source_location_t _location {};
    };

    ///////////////////////////////////////////////////////////////////////////

    class result_t {
    public:
        result_t() = default;

        inline void fail() {
            _success = false;
        }

        inline void succeed() {
            _success = true;
        }

        inline void info(
                const std::string& code,
                const std::string& message,
                const source_location_t& loc = {},
                const std::string& details = {}) {
            _messages.emplace_back(
                code,
                message,
                loc,
                details,
                result_message_t::types::info);
        }

        inline void error(
                const std::string& code,
                const std::string& message,
                const source_location_t& loc = {},
                const std::string& details = {}) {
            _messages.emplace_back(
                code,
                message,
                loc,
                details,
                result_message_t::types::error);
            fail();
        }

        inline void warning(
                const std::string& code,
                const std::string& message,
                const source_location_t& loc = {},
                const std::string& details = {}) {
            _messages.emplace_back(
                code,
                message,
                loc,
                details,
                result_message_t::types::warning);
        }

        void remove_code(const std::string& code) {
            for (auto it = _messages.begin(); it != _messages.end(); ++it) {
                if ((*it).code() == code)
                    it = _messages.erase(it);
            }
        }

        [[nodiscard]] inline bool is_failed() const {
            return !_success;
        }

        [[nodiscard]] inline bool has_code(const std::string& code) const {
            for (const auto& msg : _messages)
                if (msg.code() == code)
                    return true;
            return false;
        }

        [[nodiscard]] inline const result_message_t::list_t& messages() const {
            return _messages;
        }

        [[nodiscard]] inline const result_message_t* find_code(const std::string& code) const {
            for (const auto& _message : _messages) {
                if (_message.code() == code)
                    return &_message;
            }
            return nullptr;
        }

    private:
        bool _success = true;
        result_message_t::list_t _messages {};
    };

}