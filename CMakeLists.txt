cmake_minimum_required(VERSION 3.6)
set(TARGET games106)
project(${TARGET})

set(GFX_API VK)

find_package(Vulkan REQUIRED)
add_subdirectory(thirdparty)
add_subdirectory(homework0)