cmake_minimum_required(VERSION 3.10)

# file(GLOB entt_Headers
#     include/entt/entt.h
#     src/entt/**.h
#     src/entt/**.hpp)
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE include/)