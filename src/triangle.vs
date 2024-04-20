#version 410

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

void main()
{
    color = vertexColor;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
