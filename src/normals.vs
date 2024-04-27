#version 410

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;

out vec3 normal;

void main()
{
    gl_Position = view * model * vec4(vertexPosition, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    normal = vec3(vec4(normalMatrix * vertexNormal, 0.0));
}
