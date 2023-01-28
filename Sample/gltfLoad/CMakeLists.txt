cmake_minimum_required(VERSION 3.6)
set(TARGET gltfLoad)

add_library(gltfLoad_copy INTERFACE)

set(config
    ${CMAKE_CURRENT_SOURCE_DIR}/gltfLoadSample.json
)

copyTargetCommand("${config}" ${CMAKE_CURRENT_SOURCE_DIR}/../../bin copied_gltfLoad)
add_dependencies(gltfLoad_copy copied_gltfLoad)

set(sources
    gltfLoadSample.cpp
    gltfLoadSample.h
    Renderer.cpp
    Renderer.h
    UI.cpp
    UI.h
    stdafx.cpp
    stdafx.h
    dpiawarescaling.manifest)

source_group("Sources" FILES ${sources})

add_executable(${TARGET} WIN32 ${sources} )

target_link_libraries(${TARGET} LINK_PUBLIC gltfLoad_copy Cauldron_VK ImGUI Vulkan::Vulkan)

set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" DEBUG_POSTFIX "d")

set_target_properties(${TARGET} PROPERTIES FOLDER Sample)
