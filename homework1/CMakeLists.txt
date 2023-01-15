cmake_minimum_required(VERSION 3.6)
set(TARGET homework1)

add_library(homework1_copy INTERFACE)

set(config
    ${CMAKE_CURRENT_SOURCE_DIR}/homework1Sample.json
)

copyTargetCommand("${config}" ${CMAKE_CURRENT_SOURCE_DIR}/../bin copied_homework1)
add_dependencies(homework1_copy copied_homework1)

set(sources
    Homework1Sample.cpp
    Homework1Sample.h
    Renderer.cpp
    Renderer.h
    UI.cpp
    UI.h
    stdafx.cpp
    stdafx.h
    dpiawarescaling.manifest)

source_group("Sources" FILES ${sources})

add_executable(${TARGET} WIN32 ${sources} )

target_link_libraries(${TARGET} LINK_PUBLIC homework1_copy Cauldron_VK ImGUI Vulkan::Vulkan)

set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" DEBUG_POSTFIX "d")
