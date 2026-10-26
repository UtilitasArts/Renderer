# Basic Setup
cmake_minimum_required(VERSION 3.23)
project(Renderer VERSION 1.0 DESCRIPTION "My first SDL3 Graphics" LANGUAGES CXX)

#Add the output path after compiling.
set(
  CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>"
)
set(
  CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>"
)

add_executable(Renderer src/main.cpp)

#Apple and Linux executable alterations for RPATH
if(APPLE)
  set_target_properties(Renderer PROPERTIES
    INSTALL_RPATH "@executable_path;@loader_path"
    BUILD_WITH_INSTALL_RPATH TRUE
    MACOSX_RPATH TRUE
  )
elseif(UNIX)
  set_target_properties(Renderer PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
endif()

#Add all liraries to the project.
# Add the fpm library
add_library(
fp
libs/fpm/FixedPointMath.cpp
libs/fpm/FixedPointMath.h
)

target_include_directories(Renderer PUBLIC libs/fpm)
target_link_libraries(Renderer PUBLIC fp)

# Add the SDL libraries
set(SDLTTF_VENDORED ON) #Special Variable To add support for certain files. 
set(SDLIMAGE_AVIF OFF)  #Special Variable To remove support for certain files. 
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_ttf)

target_link_libraries(Renderer PUBLIC 
  SDL3::SDL3 
  SDL3_image::SDL3_image 
  SDL3_ttf::SDL3_ttf 
)

#Add Imgui library
add_library(
imgui STATIC
external/imgui/imgui_demo.cpp
external/imgui/imgui_draw.cpp
external/imgui/imgui_tables.cpp
external/imgui/imgui_widgets.cpp
external/imgui/imgui.cpp
external/imgui/backends/imgui_impl_sdl3.cpp
external/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC 
external/imgui
external/imgui/backends
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)
target_link_libraries(Renderer PUBLIC imgui)






