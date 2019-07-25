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

        graph/directed.h

        types.cpp types.h

        id/pool.cpp id/pool.h

        io/text.cpp io/text.h
        io/binary.cpp io/binary.h

        formatters/formatters.h

        numbers/bytes.h
        numbers/parse.cpp numbers/parse.h

        lexer/token.cpp lexer/token.h
        lexer/lexer.cpp lexer/lexer.h

        strings/transforms.cpp strings/transforms.h
        strings/escape_parser.cpp strings/escape_parser.h

        utf8/rune.cpp utf8/rune.h
        utf8/reader.cpp utf8/reader.h
        utf8/source_buffer.cpp utf8/source_buffer.h

        signals/hook.cpp signals/hook.h

        terminal/stream_builder.cpp terminal/stream_builder.h

        threading/auto_reset_event.cpp threading/auto_reset_event.h
        threading/manual_reset_event.cpp threading/manual_reset_event.h
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

add_subdirectory(lexer/tests)