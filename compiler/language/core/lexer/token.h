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

#include <compiler/types.h>
#include <compiler/data/array.h>

namespace basecode::compiler::language::core::lexer {

    using namespace std::literals;

    enum class token_type_t {
        comma,
        minus,
        caret,
        less_than,
        backslash,
        semicolon,
        directive,
        annotation,
        left_paren,
        value_sink,
        ns_keyword,
        if_keyword,
        identifier,
        use_keyword,
        nil_keyword,
        right_paren,
        for_keyword,
        in_operator,
        goto_keyword,
        with_keyword,
        else_keyword,
        enum_keyword,
        left_bracket,
        single_quote,
        line_comment,
        cast_keyword,
        case_keyword,
        true_keyword,
        proc_keyword,
        end_of_input,
        greater_than,
        add_operator,
        xor_operator,
        shl_operator,
        shr_operator,
        rol_operator,
        ror_operator,
        yield_keyword,
        union_keyword,
        break_keyword,
        defer_keyword,
        right_bracket,
        block_comment,
        block_literal,
        false_keyword,
        bind_operator,
        equal_operator,
        return_keyword,
        switch_keyword,
        string_literal,
        number_literal,
        family_keyword,
        struct_keyword,
        module_keyword,
        import_keyword,
        else_if_keyword,
        bitcast_keyword,
        divide_operator,
        modulo_operator,
        lambda_operator,
        continue_keyword,
        left_curly_brace,
        right_curly_brace,
        exponent_operator,
        multiply_operator,
        not_equal_operator,
        binary_or_operator,
        type_decl_operator,
        fallthrough_keyword,
        binary_not_operator,
        binary_and_operator,
        logical_or_operator,
        assignment_operator,
        logical_not_operator,
        logical_and_operator,
        associative_operator,
        member_select_operator,
        add_assignment_operator,
        less_than_equal_operator,
        modulo_assignment_operator,
        divide_assignment_operator,
        greater_than_equal_operator,
        multiply_assignment_operator,
        subtract_assignment_operator,
        binary_or_assignment_operator,
        binary_and_assignment_operator,
        binary_not_assignment_operator,
    };

    static inline std::string_view token_type_to_name(token_type_t type) {
        switch (type) {
            case token_type_t::comma:                           return "comma"sv;
            case token_type_t::minus:                           return "minus"sv;
            case token_type_t::caret:                           return "caret"sv;
            case token_type_t::less_than:                       return "less_than"sv;
            case token_type_t::backslash:                       return "backslash"sv;
            case token_type_t::semicolon:                       return "semicolon"sv;
            case token_type_t::directive:                       return "directive"sv;
            case token_type_t::annotation:                      return "annotation"sv;
            case token_type_t::left_paren:                      return "left_paren"sv;
            case token_type_t::value_sink:                      return "value_sink"sv;
            case token_type_t::ns_keyword:                      return "ns_keyword"sv;
            case token_type_t::if_keyword:                      return "if_keyword"sv;
            case token_type_t::identifier:                      return "identifier"sv;
            case token_type_t::use_keyword:                     return "use_keyword"sv;
            case token_type_t::nil_keyword:                     return "nil_keyword"sv;
            case token_type_t::right_paren:                     return "right_paren"sv;
            case token_type_t::for_keyword:                     return "for_keyword"sv;
            case token_type_t::in_operator:                     return "in_operator"sv;
            case token_type_t::goto_keyword:                    return "goto_keyword"sv;
            case token_type_t::with_keyword:                    return "with_keyword"sv;
            case token_type_t::else_keyword:                    return "else_keyword"sv;
            case token_type_t::enum_keyword:                    return "enum_keyword"sv;
            case token_type_t::left_bracket:                    return "left_bracket"sv;
            case token_type_t::single_quote:                    return "single_quote"sv;
            case token_type_t::line_comment:                    return "line_comment"sv;
            case token_type_t::cast_keyword:                    return "cast_keyword"sv;
            case token_type_t::case_keyword:                    return "case_keyword"sv;
            case token_type_t::true_keyword:                    return "true_keyword"sv;
            case token_type_t::proc_keyword:                    return "proc_keyword"sv;
            case token_type_t::end_of_input:                    return "end_of_input"sv;
            case token_type_t::greater_than:                    return "greater_than"sv;
            case token_type_t::add_operator:                    return "add_operator"sv;
            case token_type_t::xor_operator:                    return "xor_operator"sv;
            case token_type_t::shl_operator:                    return "shl_operator"sv;
            case token_type_t::shr_operator:                    return "shr_operator"sv;
            case token_type_t::rol_operator:                    return "rol_operator"sv;
            case token_type_t::ror_operator:                    return "ror_operator"sv;
            case token_type_t::bind_operator:                   return "bind_operator"sv;
            case token_type_t::yield_keyword:                   return "yield_keyword"sv;
            case token_type_t::union_keyword:                   return "union_keyword"sv;
            case token_type_t::break_keyword:                   return "break_keyword"sv;
            case token_type_t::defer_keyword:                   return "defer_keyword"sv;
            case token_type_t::right_bracket:                   return "right_bracket"sv;
            case token_type_t::block_comment:                   return "block_comment"sv;
            case token_type_t::block_literal:                   return "block_literal"sv;
            case token_type_t::false_keyword:                   return "false_keyword"sv;
            case token_type_t::return_keyword:                  return "return_keyword"sv;
            case token_type_t::switch_keyword:                  return "switch_keyword"sv;
            case token_type_t::string_literal:                  return "string_literal"sv;
            case token_type_t::number_literal:                  return "number_literal"sv;
            case token_type_t::family_keyword:                  return "family_keyword"sv;
            case token_type_t::struct_keyword:                  return "struct_keyword"sv;
            case token_type_t::module_keyword:                  return "module_keyword"sv;
            case token_type_t::import_keyword:                  return "import_keyword"sv;
            case token_type_t::equal_operator:                  return "equal_operator"sv;
            case token_type_t::else_if_keyword:                 return "else_if_keyword"sv;
            case token_type_t::bitcast_keyword:                 return "bitcast_keyword"sv;
            case token_type_t::divide_operator:                 return "divide_operator"sv;
            case token_type_t::modulo_operator:                 return "modulo_operator"sv;
            case token_type_t::lambda_operator:                 return "lambda_operator"sv;
            case token_type_t::continue_keyword:                return "continue_keyword"sv;
            case token_type_t::left_curly_brace:                return "left_curly_brace"sv;
            case token_type_t::right_curly_brace:               return "right_curly_brace"sv;
            case token_type_t::exponent_operator:               return "exponent_operator"sv;
            case token_type_t::multiply_operator:               return "multiply_operator"sv;
            case token_type_t::not_equal_operator:              return "not_equal_operator"sv;
            case token_type_t::binary_or_operator:              return "binary_or_operator"sv;
            case token_type_t::type_decl_operator:              return "type_decl_operator"sv;
            case token_type_t::fallthrough_keyword:             return "fallthrough_keyword"sv;
            case token_type_t::binary_not_operator:             return "binary_not_operator"sv;
            case token_type_t::binary_and_operator:             return "binary_and_operator"sv;
            case token_type_t::logical_or_operator:             return "logical_or_operator"sv;
            case token_type_t::assignment_operator:             return "assignment_operator"sv;
            case token_type_t::logical_not_operator:            return "logical_not_operator"sv;
            case token_type_t::logical_and_operator:            return "logical_and_operator"sv;
            case token_type_t::associative_operator:            return "associative_operator"sv;
            case token_type_t::member_select_operator:          return "member_select_operator"sv;
            case token_type_t::add_assignment_operator:         return "add_assignment_operator"sv;
            case token_type_t::less_than_equal_operator:        return "less_than_equal_operator"sv;
            case token_type_t::modulo_assignment_operator:      return "modulo_assignment_operator"sv;
            case token_type_t::divide_assignment_operator:      return "divide_assignment_operator"sv;
            case token_type_t::greater_than_equal_operator:     return "greater_than_equal_operator"sv;
            case token_type_t::multiply_assignment_operator:    return "multiply_assignment_operator"sv;
            case token_type_t::subtract_assignment_operator:    return "subtract_assignment_operator"sv;
            case token_type_t::binary_or_assignment_operator:   return "binary_or_assignment_operator"sv;
            case token_type_t::binary_and_assignment_operator:  return "binary_and_assignment_operator"sv;
            case token_type_t::binary_not_assignment_operator:  return "binary_not_assignment_operator"sv;
            default:
                return "unknown"sv;
        }
    }

    struct token_t final {
        token_type_t type{};
        std::string_view value{};
    };

    ///////////////////////////////////////////////////////////////////////////
    
    enum class number_type_t {
        none,
        integer,
        arbitrary,
        floating_point
    };

    static inline std::string_view number_type_to_name(number_type_t type) {
        switch (type) {
            case number_type_t::none:           return "none"sv;
            case number_type_t::integer:        return "integer"sv;
            case number_type_t::arbitrary:      return "arbitrary"sv;
            case number_type_t::floating_point: return "floating_point"sv;
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    enum class number_size_t {
        byte,
        word,
        dword,
        qword
    };

    static inline std::string_view number_size_to_name(number_size_t size) {
        switch (size) {
            case number_size_t::byte:  return "byte"sv;
            case number_size_t::word:  return "word"sv;
            case number_size_t::dword: return "dword"sv;
            case number_size_t::qword: return "qword"sv;
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    struct number_token_t final {
        bool is_signed{};
        bool imaginary{};
        uint8_t radix = 10;
        number_type_t type{};
        number_size_t size{};
        union {
            float f32;
            double f64;
            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            uint64_t u64;
        } value{.u64 = 0};
    };

    void apply_narrowed_value(
            number_token_t& token, 
            number_size_t size, 
            double value);

    void apply_narrowed_value(
            number_token_t& token, 
            number_size_t size, 
            int64_t value, 
            bool check_sign_bit = true);

    std::optional<number_size_t> narrow_type(double value);

    std::optional<number_size_t> narrow_type(int64_t value);

    ///////////////////////////////////////////////////////////////////////////

    struct line_comment_token_t final {};

    struct block_comment_token_t final {
        explicit block_comment_token_t(memory::allocator_t* allocator) : children(allocator) {}
        std::string_view capture{};
        data::array_t<block_comment_token_t> children;
    };

    ///////////////////////////////////////////////////////////////////////////

    struct block_literal_token_t final {};

    struct string_literal_token_t final {};

}
