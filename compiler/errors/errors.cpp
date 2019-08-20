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

#include <compiler/data/hash_table.h>
#include "errors.h"

namespace basecode::compiler::errors {

    struct error_system_t final {
        explicit error_system_t(
            memory::allocator_t* allocator,
            std::initializer_list<std::pair<error_decl_key_t, error_decl_t>> elements) : allocator(allocator),
                                                                                         decls(elements, allocator) {
        }

        memory::allocator_t* allocator;
        data::hash_table_t<error_decl_key_t, error_decl_t> decls;
    };

    error_system_t* g_error_system{};

    bool initialize(
            result_t& r,
            memory::allocator_t* allocator) {
        auto p = allocator->allocate(sizeof(error_system_t), alignof(error_system_t));
        g_error_system = new (p) error_system_t(
            allocator,
            {
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
            });
        return true;
    }

    bool shutdown(result_t& r) {
        auto allocator = g_error_system->allocator;
        g_error_system->~error_system_t();
        allocator->deallocate(g_error_system);
        return true;
    }

    error_decl_t* find_decl(error_code_t code) {
        std::locale locale;

        // XXX: this sucks.  come back and spend time on it.
        auto name = locale.name();
        if (name == "C")
            name = "en_US";

        error_decl_key_t key{name, code};
        return g_error_system->decls.find(key);
    }

}