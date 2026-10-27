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

add_executable(Renderer
#main-project
src/main.cpp
#Fixed Point Math Library
libs/fpm/FixedPointMath.cpp
#Imgui Library
external/imgui/imgui_demo.cpp
external/imgui/imgui_draw.cpp
external/imgui/imgui_tables.cpp
external/imgui/imgui_widgets.cpp
external/imgui/imgui.cpp
external/imgui/backends/imgui_impl_sdl3.cpp
external/imgui/backends/imgui_impl_sdlrenderer3.cpp
external/imgui/backends/imgui_impl_sdlgpu3.cpp
)

target_include_directories(Renderer PUBLIC
libs/fpm
external/imgui
external/imgui/backends
)

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
# Add the SDL libraries
set(SDLTTF_VENDORED ON) #Special Variable To add support for certain files. 
set(SDLIMAGE_AVIF OFF)  #Special Variable To remove support for certain files. 

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_ttf)

#Add wxwidgets 
add_subdirectory(external/wxWidgets)

target_link_libraries(Renderer PUBLIC 
  SDL3::SDL3 
  SDL3_image::SDL3_image 
  SDL3_ttf::SDL3_ttf 
  wx::net
  wx::core
  wx::base
)







