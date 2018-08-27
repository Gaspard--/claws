set(MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/claws/container)

set(MODULE_PUBLIC_HEADERS
        "${MODULE_PATH}/array_ops.hpp"
        "${MODULE_PATH}/container_view.hpp"
        "${MODULE_PATH}/contextful_container.hpp"
        "${MODULE_PATH}/iterator_pair.hpp"
        "${MODULE_PATH}/vect.hpp"
        )

set(MODULE_PRIVATE_HEADERS
        "")

set(MODULE_SOURCES
        ${MODULE_PUBLIC_HEADERS}
        ${MODULE_PRIVATE_HEADERS}
        )
