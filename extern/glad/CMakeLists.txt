cmake_minimum_required(VERSION 3.10)

# GLAD
set(GLAD_Headers
include/glad/glad.h
include/KHR/khrplatform.h)

set(GLAD_Sources src/glad.c)

add_library(glad STATIC ${GLAD_Headers} ${GLAD_Sources})

target_include_directories(glad INTERFACE include)
target_include_directories(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)