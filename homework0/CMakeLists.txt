cmake_minimum_required(VERSION 3.6)
set(TARGET homework0)

set(sources
    main.cpp)

source_group("Sources" FILES ${sources})

add_executable(${TARGET} ${sources})

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET} PUBLIC homeworkCommon)

set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/bin" DEBUG_POSTFIX "d")