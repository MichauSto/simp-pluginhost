project(simp_pluginhost_api)

file(GLOB_RECURSE src pluginhost/*.hpp pluginhost/*.cpp)

add_library(simp_pluginhost_api INTERFACE ${src})
set_property(TARGET simp_pluginhost_api PROPERTY CXX_STANDARD 20)
target_include_directories(simp_pluginhost_api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})