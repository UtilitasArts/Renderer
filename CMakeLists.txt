cmake_minimum_required(VERSION 4.5)
project(Renderer VERSION 1.0 DESCRIPTION "My first SDL3 Graphics" LANGUAGES CXX)
add_executable(Renderer src/main.cpp)

add_subdirectory(libs/fpm)
target_include_directories(Renderer PUBLIC libs/fpm)
target_link_libraries(Renderer PUBLIC fpm)




