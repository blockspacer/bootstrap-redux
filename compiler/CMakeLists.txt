cmake_minimum_required(VERSION 3.15)
project(compiler)

message(STATUS "${PROJECT_NAME} build configuration:")

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (PRODUCT_NAME  "Basecode Compiler Library")

configure_file (
        "${PROJECT_SOURCE_DIR}/configure.in.h"
        "${PROJECT_BINARY_DIR}/configure.h"
)

add_library(
        ${PROJECT_NAME}

        defer.h

        data/array.h
        data/directed_graph.h
        data/hash_table.h

        types.cpp types.h

        memory/pool.h
        memory/allocator.h
        memory/temp_allocator.h
        memory/block_allocator.h
        memory/system.cpp memory/system.h
        memory/system_allocator.cpp memory/system_allocator.h
        memory/scratch_allocator.cpp memory/scratch_allocator.h

        id/pool.cpp id/pool.h

        io/text.cpp io/text.h
        io/binary.cpp io/binary.h

        formatters/formatters.h

        errors/errors.cpp errors/errors.h

        signals/hook.cpp signals/hook.h

        numbers/bytes.h
        numbers/parse.cpp numbers/parse.h

        hashing/murmur.cpp hashing/murmur.h

        utf8/rune.cpp utf8/rune.h
        utf8/reader.cpp utf8/reader.h
        utf8/source_buffer.cpp utf8/source_buffer.h

        strings/pool.cpp strings/pool.h
        strings/transforms.cpp strings/transforms.h
        strings/escape_parser.cpp strings/escape_parser.h

        terminal/stream_factory.cpp terminal/stream_factory.h

        language/core/ast/ast.cpp language/core/ast/ast.h
        language/core/lexer/token.cpp language/core/lexer/token.h
        language/core/lexer/lexer.cpp language/core/lexer/lexer.h
        language/core/parser/parser.cpp language/core/parser/parser.h

        threading/auto_reset_event.cpp threading/auto_reset_event.h
        threading/manual_reset_event.cpp threading/manual_reset_event.h

        workspace/session.cpp workspace/session.h
)
target_link_libraries(
        ${PROJECT_NAME}
        utf8proc
        fmt-header-only
        boost-system
        boost-filesystem
        tcc
        mstch
)

add_subdirectory(language/core/lexer/tests)
add_subdirectory(language/core/parser/tests)