cmake_minimum_required(VERSION 3.6)
set(TARGET homeworkCommon)
project(${TARGET})

set(sources
    stdafx.cpp
    stdafx.h)

source_group("Sources" FILES ${sources})

add_library(${TARGET} STATIC ${sources})

target_link_libraries(${TARGET} PUBLIC Cauldron_VK ImGUI Vulkan::Vulkan)

set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" DEBUG_POSTFIX "d")
