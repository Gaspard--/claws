set(MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/claws/utils)

set(MODULE_PUBLIC_HEADERS
        "${MODULE_PATH}/array_ops.hpp"
        "${MODULE_PATH}/box.hpp"
        "${MODULE_PATH}/circular_iterator.hpp"
        "${MODULE_PATH}/constexpr_algorithm.hpp"
        "${MODULE_PATH}/contextful_container.hpp"
        "${MODULE_PATH}/handle_types.hpp"
        "${MODULE_PATH}/iterator_util.hpp"
        "${MODULE_PATH}/lambda_ops.hpp"
        "${MODULE_PATH}/lambda_utils.hpp"
        "${MODULE_PATH}/on_scope_exit.hpp"
        "${MODULE_PATH}/self_iterator.hpp"
        "${MODULE_PATH}/tagged_data.hpp"
        "${MODULE_PATH}/tuple_helper.hpp"
        "${MODULE_PATH}/type.hpp"
        "${MODULE_PATH}/vect.hpp"
        )

set(MODULE_PRIVATE_HEADERS
        "")

set(MODULE_SOURCES
        ${MODULE_PUBLIC_HEADERS}
        ${MODULE_PRIVATE_HEADERS}
        )
