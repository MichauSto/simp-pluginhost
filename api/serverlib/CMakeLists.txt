project(simp_pluginhost_api)

file(GLOB_RECURSE src include/*.hpp source/*.hpp source/*.cpp)

add_library(simp_pluginhost_serverlib STATIC ${src})
set_property(TARGET simp_pluginhost_serverlib PROPERTY CXX_STANDARD 20)
target_link_libraries(simp_pluginhost_serverlib PRIVATE simp_pluginhost_api fmt)
target_include_directories(simp_pluginhost_serverlib PRIVATE source)
target_include_directories(simp_pluginhost_serverlib PUBLIC include)