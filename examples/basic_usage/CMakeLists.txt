cmake_minimum_required(VERSION 3.10)

add_executable(basic_usage main.cpp)
target_link_libraries(basic_usage team4_game_engine)
target_link_libraries(basic_usage entt)
target_link_libraries(basic_usage spdlog)
target_link_libraries(basic_usage glad)
target_link_libraries(basic_usage glfw)
target_link_libraries(basic_usage dear_imgui)
SET(RESSOURCES
	particuleShader.vs
	particuleShader.fs
	shader.vs
	shader.fs
)
FILE(COPY ${RESSOURCES} DESTINATION ${CMAKE_BINARY_DIR}/examples/basic_usage)