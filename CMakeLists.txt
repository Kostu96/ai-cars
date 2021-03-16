cmake_minimum_required(VERSION 3.18)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(si-proj LANGUAGES CXX)
set(TARGET_NAME cars-ai)

add_subdirectory(source)

add_executable(${TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    ${SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${SOURCES})

target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()

add_subdirectory(third_party)

target_link_libraries(${TARGET_NAME} PRIVATE
    sfml-graphics
    box2d
    imgui
)
