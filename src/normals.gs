#version 410

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 normal[];

void main()
{
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(normal[0], 0.0) * 0.3;
    EmitVertex();
    EndPrimitive();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position + vec4(normal[1], 0.0) * 0.3;
    EmitVertex();
    EndPrimitive();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position + vec4(normal[2], 0.0) * 0.3;
    EmitVertex();
    EndPrimitive();
}
