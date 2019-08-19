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

#include <string_view>
#include <compiler/types.h>
#include <compiler/data/hash_table.h>
#include <compiler/memory/object_pool.h>
#include "model.h"
#include "attribute.h"

namespace basecode::compiler::graphviz {

    class attribute_container_t final {
    public:
        attribute_container_t(
            memory::allocator_t* allocator,
            model_t* model,
            component_type_t type);

        [[nodiscard]] component_type_t type() const;

        [[nodiscard]] attribute_value_list_t values() const;

        bool set_value(result_t& r, attribute_type_t attr, bool v);

        bool get_value(result_t& r, attribute_type_t attr, bool& v);

        bool set_value(result_t& r, attribute_type_t attr, double v);

        bool get_value(result_t& r, attribute_type_t attr, double& v);

        bool set_value(result_t& r, attribute_type_t attr, int32_t v);

        bool get_value(result_t& r, attribute_type_t attr, int32_t& v);

        bool get_value(result_t& r, attribute_type_t attr, data::string_t& v);

        bool set_value(result_t& r, attribute_type_t attr, const data::string_t& v);

    private:
        model_t* _model;
        component_type_t _type;
        memory::object_pool_t _storage;
        memory::allocator_t* _allocator;
        data::hash_table_t<attribute_type_t, attribute_value_t*> _values;
    };

}