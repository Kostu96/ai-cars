set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/debug_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/debug_draw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/imgui_sfml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/imgui_sfml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(SOURCES ${SOURCES} PARENT_SCOPE)
