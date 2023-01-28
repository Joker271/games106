cmake_minimum_required(VERSION 3.6)
set(TARGET HelloTriangle)

add_library(HelloTriangle_copy INTERFACE)

set(config
    ${CMAKE_CURRENT_SOURCE_DIR}/hello_triangle_vert.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/hello_triangle_frag.glsl
)

copyTargetCommand("${config}" ${CMAKE_CURRENT_SOURCE_DIR}/../bin/ShaderLibVK/HelloTriangle copied_HelloTriangle)
add_dependencies(HelloTriangle_copy copied_HelloTriangle)

set(sources
    main.cpp
    stdafx.cpp
    stdafx.h
)

source_group("Sources" FILES ${sources})

add_executable(${TARGET} WIN32 ${sources})

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET} PUBLIC HelloTriangle_copy Cauldron_VK ImGUI Vulkan::Vulkan)

set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" DEBUG_POSTFIX "d")