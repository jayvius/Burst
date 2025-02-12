#pragma once

#include <vector>

#include "GL/gl3w.h"
#include "types.hpp"

struct VertexIndex {GLuint asGLuint;};

struct Buffer
{
    GLuint vao;
    GLuint vertexBufferID;
    GLuint indexBufferID;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    size_t vertexBufferUploadSize;
    size_t indexBufferUploadSize;
    size_t numVertices;
    size_t numObjects;
};

void init(Buffer &buffer);
VertexIndex addVertex(Buffer &buffer, Vertex &vertex);
VertexIndex addVertex(Buffer &buffer, Vertex &&vertex);
void addTriangle(Buffer &buffer, VertexIndex index1, VertexIndex index2, VertexIndex index3);
void draw(Buffer &buffer);
void exportObj(Buffer &buffer);
