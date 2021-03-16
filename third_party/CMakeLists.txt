set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
add_subdirectory(box2d)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
add_subdirectory(SFML)

set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui.h
	imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)
    
add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui
    PUBLIC imgui)

set_target_properties(
    box2d
    imgui
    sfml-graphics
    sfml-main
    sfml-system
    sfml-window
    PROPERTIES
    FOLDER third_party
)
