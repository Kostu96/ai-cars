set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/car/car.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/car/car.h
    ${CMAKE_CURRENT_SOURCE_DIR}/car/wheel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/car/wheel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/debug_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/debug_draw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/imgui_sfml.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/imgui_sfml.h
    ${CMAKE_CURRENT_SOURCE_DIR}/application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/track.h
)

set(SOURCES ${SOURCES} PARENT_SCOPE)
