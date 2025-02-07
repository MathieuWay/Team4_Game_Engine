#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in mat4 model;

out vec3 VertexColor;

uniform mat4 view;
uniform mat4 projection;

void main (){
    gl_Position = projection * view * model * vec4(a_Pos, 1.0);
    VertexColor = a_Color;
}