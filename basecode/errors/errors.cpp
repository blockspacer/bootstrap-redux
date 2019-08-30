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

#include <basecode/adt/hash_table.h>
#include <basecode/strings/transforms.h>
#include <basecode/memory/frame_allocator.h>
#include "errors.h"

namespace basecode::errors {

    struct error_system_t final {
        error_system_t(
            memory::allocator_t* allocator,
            error_decl_init_list_t elements) : locale(allocator),
                                               allocator(allocator),
                                               decls(elements, allocator) {
        }

        adt::string_t locale;
        strings::pool_t* pool{};
        memory::allocator_t* allocator;
        memory::frame_allocator_t<4096>* pool_allocator{};
        adt::hash_table_t<error_decl_key_t, error_decl_t> decls;
    };

    error_system_t* g_error_system{};

    bool shutdown() {
        auto allocator = g_error_system->allocator;

        memory::destroy(allocator, g_error_system->pool);
        memory::destroy(allocator, g_error_system->pool_allocator);
        memory::destroy(allocator, g_error_system);

        return true;
    }

    bool initialize(
            result_t& r,
            memory::allocator_t* allocator) {
        const error_decl_init_list_t elements = {
            // ----------------------
            // io
            // ----------------------

            {
                {.code = io::unable_to_read_file, .locale = "en_US"},
                {.code = "I001", .message = "unable to read file: {}"}
            },

            {
                {.code = io::unable_to_write_file, .locale = "en_US"},
                {.code = "I002", .message = "unable to write file: {}"}
            },

            // ----------------------
            // lexer_t
            // ----------------------

            {
                {.code = lexer::unable_to_convert_integer_value, .locale = "en_US"},
                {.code = "L001", .message = "unable to convert integer value {} because {}"}
            },

            {
                {.code = lexer::unable_to_narrow_integer_value, .locale = "en_US"},
                {.code = "L002", .message = "unable to narrow integer value"}
            },

            {
                {.code = lexer::unable_to_convert_floating_point_value, .locale = "en_US"},
                {.code = "L003", .message = "unable to convert floating point value {} because {}"}
            },

            {
                {.code = lexer::unable_to_narrow_floating_point_value, .locale = "en_US"},
                {.code = "L004", .message = "unable to narrow floating point value"}
            },

            {
                {.code = lexer::invalid_identifier_start_character, .locale = "en_US"},
                {.code = "L005", .message = "identifiers must start with _ or a letter; found: {}"}
            },

            {
                {.code = lexer::expected_identifier, .locale = "en_US"},
                {.code = "L006", .message = "expected identifier"}
            },

            {
                {.code = lexer::unexpected_end_of_input, .locale = "en_US"},
                {.code = "L007", .message = "unexpected end of input"}
            },

            {
                {.code = lexer::unexpected_decimal_point, .locale = "en_US"},
                {.code = "L008", .message = "unexpected decimal point"}
            },

            {
                {.code = lexer::expected_closing_single_quote, .locale = "en_US"},
                {.code = "L009", .message = "expected closing ' but found: {}"}
            },

            {
                {.code = lexer::expected_directive_prefix, .locale = "en_US"},
                {.code = "L010", .message = "expected directive prefix: #"}
            },

            {
                {.code = lexer::expected_annotation_prefix, .locale = "en_US"},
                {.code = "L011", .message = "expected annotation prefix: @"}
            },

            {
                {.code = lexer::exponent_notation_not_valid_for_integers, .locale = "en_US"},
                {.code = "L012", .message = "exponent notation is not valid for integer literals"}
            },

            {
                {.code = lexer::unexpected_letter_after_decimal_number_literal, .locale = "en_US"},
                {.code = "L013", .message = "unexpected letter immediately after decimal number"}
            },

            {
                {.code = lexer::expected_hex_literal_prefix, .locale = "en_US"},
                {.code = "L014", .message = "expected hex prefix: $"}
            },

            {
                {.code = lexer::unexpected_letter_after_hexadecimal_number_literal, .locale = "en_US"},
                {.code = "L015", .message = "unexpected letter immediately after hexadecimal number"}
            },

            {
                {.code = lexer::expected_octal_literal_prefix, .locale = "en_US"},
                {.code = "L016", .message = "expected octal prefix: @"}
            },

            {
                {.code = lexer::unexpected_letter_after_octal_number_literal, .locale = "en_US"},
                {.code = "L017", .message = "unexpected letter immediately after octal number"}
            },

            {
                {.code = lexer::expected_binary_literal_prefix, .locale = "en_US"},
                {.code = "L018", .message = "expected binary prefix: %"}
            },

            {
                {.code = lexer::unexpected_letter_after_binary_number_literal, .locale = "en_US"},
                {.code = "L019", .message = "unexpected letter or non-binary digit immediately after binary number"}
            },

            {
                {.code = lexer::expected_closing_block_literal, .locale = "en_US"},
                {.code = "L020", .message = "expected }} but found: {}"}
            },

            {
                {.code = lexer::unescaped_quote, .locale = "en_US"},
                {.code = "L021", .message = "an unescaped quote was detected"}
            },

            // ----------------------
            // parser_t
            // ----------------------

            {
                {.code = parser::invalid_token, .locale = "en_US"},
                {.code = "P001", .message = "a token has not been properly configured for parsing"}
            },

            {
                {.code = parser::undefined_production_rule, .locale = "en_US"},
                {.code = "P002", .message = "undefined production rule"}
            },

            {
                {.code = parser::missing_operator_production_rule, .locale = "en_US"},
                {.code = "P003", .message = "missing operator production rule"}
            },

            {
                {.code = parser::unexpected_token, .locale = "en_US"},
                {.code = "P004", .message = "expected token {} but encountered {}"}
            },

            {
                {.code = parser::member_select_operator_requires_identifier_lvalue, .locale = "en_US"},
                {.code = "P005", .message = "member select requires identifier lvalue"}
            },

            {
                {.code = parser::member_select_operator_requires_identifier_rvalue, .locale = "en_US"},
                {.code = "P006", .message = "member select requires identifier rvalue"}
            },

            {
                {.code = parser::expected_expression, .locale = "en_US"},
                {.code = "P007", .message = "expected expression"}
            },

            {
                {.code = parser::invalid_assignment_expression, .locale = "en_US"},
                {.code = "P008", .message = "invalid assignment expression"}
            },

            // ----------------------
            // graphviz
            // ----------------------

            {
                {.code = graphviz::attribute_type_not_found, .locale = "en_US"},
                {.code = "G001", .message = "attribute type not found"}
            },

            {
                {.code = graphviz::invalid_attribute_for_component, .locale = "en_US"},
                {.code = "G002", .message = "invalid attriubte '{}' for component '{}'"}
            },

            // ----------------------
            // source_buffer_t
            // ----------------------

            {
                {.code = source_buffer::unable_to_open_file, .locale = "en_US"},
                {.code = "S001", .message = "unable to open source file: {}"}
            },

            // ----------------------
            // utf8_module
            // ----------------------

            {
                {.code = utf8_module::unable_to_open_file, .locale = "en_US"},
                {.code = "S001", .message = "unable to open source file: {}"}
            },

            {
                {.code = utf8_module::at_end_of_buffer, .locale = "en_US"},
                {.code = "S002", .message = "at end of buffer"}
            },

            {
                {.code = utf8_module::at_beginning_of_buffer, .locale = "en_US"},
                {.code = "S003", .message = "at beginning of buffer"}
            },

            {
                {.code = utf8_module::illegal_encoding, .locale = "en_US"},
                {.code = "S004", .message = "illegal utf-8 encoding"}
            },

            {
                {.code = utf8_module::illegal_nul_character, .locale = "en_US"},
                {.code = "S005", .message = "illegal nul character"}
            },

            {
                {.code = utf8_module::illegal_byte_order_mark, .locale = "en_US"},
                {.code = "S006", .message = "illegal byte-order mark"}
            }
        };

        g_error_system = memory::construct_with_allocator<error_system_t>(allocator, allocator, elements);

        g_error_system->pool_allocator = memory::construct_with_allocator<memory::frame_allocator_t<4096>>(
            g_error_system->allocator,
            g_error_system->allocator);

        g_error_system->pool = memory::construct_with_allocator<strings::pool_t>(
            g_error_system->allocator,
            g_error_system->pool_allocator);

        setlocale(LC_ALL, "");
        string_t locale = setlocale(LC_CTYPE, nullptr);
        auto parts = strings::string_to_list(locale, '.');
        if (parts.empty()) {
            g_error_system->locale = "en_US";
        } else {
            g_error_system->locale = parts[0];
        }

        return true;
    }

    strings::pool_t& pool() {
        return *g_error_system->pool;
    }

    error_decl_t* find_decl(error_code_t code) {
        error_decl_key_t key{g_error_system->locale, code};
        return g_error_system->decls.find(key);
    }

}

template<>
uint64_t basecode::adt::hash_key(basecode::errors::error_decl_key_t key) {
    const auto composite_key = fmt::format("{}:{}", key.locale, key.code);
    return hashing::murmur::hash64(composite_key.data(), composite_key.size());
}
