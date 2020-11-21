cmake_minimum_required(VERSION 3.10)

# Dear ImGui
add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)
set(Dear_ImGui_Headers
    include/dear_imgui/imconfig.h
    include/dear_imgui/imgui.h
    include/dear_imgui/imgui_internal.h
    include/dear_imgui/imgui_impl_glfw.h
    include/dear_imgui/imgui_impl_opengl3.h
    include/dear_imgui/imstb_rectpack.h
    include/dear_imgui/imstb_textedit.h
    include/dear_imgui/imstb_truetype.h
    include/dear_imgui/ImGuizmo.h
    include/dear_imgui/ImSequencer.h
    include/dear_imgui/ImCurveEdit.h
    include/dear_imgui/ImGradient.h)

set(Dear_ImGui_Sources
    src/imgui.cpp
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
    src/imgui_widgets.cpp
    src/ImGuizmo.cpp
    src/ImSequencer.cpp
    src/ImCurveEdit.cpp
    src/ImGradient.cpp)

add_library(dear_imgui STATIC ${Dear_ImGui_Headers} ${Dear_ImGui_Sources})
target_include_directories(dear_imgui INTERFACE include)
target_include_directories(dear_imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(dear_imgui PRIVATE glad)
target_link_libraries(dear_imgui PRIVATE glfw)