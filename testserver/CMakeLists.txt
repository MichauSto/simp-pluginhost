file(GLOB_RECURSE src source/*.hpp source/*.cpp)

project(simp_pluginhost_server)
add_executable(simp_pluginhost_server ${src})
set_property(TARGET simp_pluginhost_server PROPERTY CXX_STANDARD 20)
add_dependencies(simp_pluginhost_server simp_pluginhost)
target_link_libraries(simp_pluginhost_server PUBLIC simp_pluginhost_serverlib)
target_include_directories(simp_pluginhost_server PUBLIC source)
target_compile_definitions(simp_pluginhost_server PUBLIC SPH_EXECUTABLE=\"$<TARGET_FILE:simp_pluginhost>\" SPH_OMSI_DIR=\"${OMSI_DIR_DEBUG}\")