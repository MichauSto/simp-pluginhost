cmake_minimum_required(VERSION 3.22)

set(OMSI_DIR_DEBUG "C:/Program Files (x86)/Steam/steamapps/common/OMSI 2" CACHE STRING "OMSI 2 Directory for Visual Studio debug sessions")

add_subdirectory(dependencies/fmt)
add_subdirectory(api)

file(GLOB_RECURSE src source/*.hpp source/*.cpp)

project(simp_pluginhost)
add_executable(simp_pluginhost ${src})
set_property(TARGET simp_pluginhost PROPERTY CXX_STANDARD 20)
target_link_libraries(simp_pluginhost PUBLIC simp_pluginhost_api)
target_include_directories(simp_pluginhost PUBLIC source)
target_compile_definitions(simp_pluginhost PUBLIC SPH_IS_PLUGIN)

add_subdirectory(testserver)

add_custom_command(TARGET simp_pluginhost POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:simp_pluginhost>
  ${CMAKE_CURRENT_SOURCE_DIR}/Host_$<CONFIG>/$<TARGET_FILE_NAME:simp_pluginhost>)