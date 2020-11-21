#version 330 core

layout(location = 0) out vec4 color;

in vec3 VertexColor;

void main (){
   color = vec4(VertexColor, 1.0f);
}