#include "buffer.hpp"
#include <fmt/core.h>

const size_t VERTEX_BUFFER_SIZE = 10 * 1024 * 1024 * sizeof(Vertex);
const size_t INDEX_BUFFER_SIZE = 10 * 1024 * 1024 * sizeof(GLuint);

void init(Buffer &buffer)
{
    glGenVertexArrays(1, &buffer.vao);
    glBindVertexArray(buffer.vao);

    glGenBuffers(1, &buffer.vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertexBufferID);

    glGenBuffers(1, &buffer.indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.indexBufferID);

    GLuint positionAttribute = 0;
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(positionAttribute);

    GLuint normalAttribute = 1;
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(normalAttribute);

    GLuint colorAttribute = 2;
    glVertexAttribPointer(colorAttribute, 4, GL_UNSIGNED_BYTE, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(colorAttribute);

    glBufferData(GL_ARRAY_BUFFER, VERTEX_BUFFER_SIZE, nullptr, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_BUFFER_SIZE, nullptr, GL_STATIC_DRAW);

    buffer.vertexBufferUploadSize = 0;
    buffer.indexBufferUploadSize = 0;
    buffer.numVertices = 0;
    buffer.numObjects = 0;

    glBindVertexArray(0);
}

VertexIndex addVertex(Buffer &buffer, Vertex &vertex)
{
    assert (buffer.vertexBufferUploadSize <= VERTEX_BUFFER_SIZE);
    buffer.vertices.push_back(vertex);
    return VertexIndex{static_cast<GLuint>(buffer.vertices.size() - 1)};
}

VertexIndex addVertex(Buffer &buffer, Vertex &&vertex)
{
    return addVertex(buffer, vertex);
}

void addTriangle(Buffer &buffer, VertexIndex index1, VertexIndex index2, VertexIndex index3)
{
    assert (buffer.indexBufferUploadSize + 3 <= INDEX_BUFFER_SIZE);
    buffer.indices.push_back(index1.asGLuint);
    buffer.indices.push_back(index2.asGLuint);
    buffer.indices.push_back(index3.asGLuint);
}

void draw(Buffer &buffer)
{
    glBindVertexArray(buffer.vao);
    if (!buffer.vertices.empty()) {
        size_t bufferSize = sizeof(Vertex) * buffer.vertices.size();
        glBufferSubData(GL_ARRAY_BUFFER, buffer.vertexBufferUploadSize, bufferSize, buffer.vertices.data());
        buffer.vertexBufferUploadSize += bufferSize;
        buffer.vertices.clear();
    }
    if (!buffer.indices.empty()) {
        size_t bufferSize = sizeof(GLuint) * buffer.indices.size();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, buffer.indexBufferUploadSize, bufferSize, buffer.indices.data());
        buffer.indexBufferUploadSize += bufferSize;
        buffer.numVertices += buffer.indices.size();
        buffer.indices.clear();
    }

    glDrawElements(GL_TRIANGLES, buffer.numVertices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
