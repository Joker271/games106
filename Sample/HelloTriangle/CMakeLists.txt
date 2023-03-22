cmake_minimum_required(VERSION 3.6)
set(TARGET HelloTriangle)

include(shader-link)

set(sources
    main.cpp
    stdafx.cpp
    stdafx.h
)

source_group("Sources" FILES ${sources})

add_executable(${TARGET} WIN32 ${sources})

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET} PUBLIC Cauldron_VK ImGUI Vulkan::Vulkan)


add_custom_command(
    TARGET ${TARGET} PRE_BUILD
    ${REMOVE_SHADER}
    ${LINK_SHADER_DIRECTORIES}
    VERBATIM
)

set_property(
    TARGET ${TARGET}
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin
)

set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" DEBUG_POSTFIX "d")

set_target_properties(${TARGET} PROPERTIES FOLDER Sample)
