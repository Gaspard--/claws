set(MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/claws/iterator)

set(MODULE_PUBLIC_HEADERS
        "${MODULE_PATH}/circular_iterator.hpp"
        "${MODULE_PATH}/iterator_view.hpp"
        "${MODULE_PATH}/same_val_iterator.hpp"
        "${MODULE_PATH}/self_iterator.hpp"
        )

set(MODULE_PRIVATE_HEADERS
        "")

set(MODULE_SOURCES
        ${MODULE_PUBLIC_HEADERS}
        ${MODULE_PRIVATE_HEADERS}
        )
