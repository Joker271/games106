get_filename_component(ROOT_DIR .. ABSOLUTE BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
list(APPEND CMAKE_MODULE_PATH ${ROOT_DIR}/cmake)

add_subdirectory(HelloTriangle)
add_subdirectory(gltfLoad)
#add_subdirectory(PNTriangle)
