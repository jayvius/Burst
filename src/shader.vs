#version 410

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vec3 normal = vec3(vec4(normalMatrix * vertexNormal, 0.0));

    // Calculate light intensity from light sources
    float light1 = max(dot(normal, vec3(0.0, 1.0, 0.0)), 0.0) * 0.7;
    float light2 = max(dot(normal, vec3(0.707, 0.0, 0.707)), 0.0) * 0.5;
    float light3 = max(dot(normal, vec3(-0.707, 0.0, 0.707)), 0.0) * 0.3;

    // Calculate final color from lighting sources
    color = vec4(light1 * vec3(vertexColor) + light2 * vec3(vertexColor) + light3 * vec3(vertexColor), vertexColor.a);

    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
