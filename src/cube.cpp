#include "cube.hpp"
#include <array>

const size_t RBF = 0;
const size_t RTF = 1;
const size_t LTF = 2;
const size_t LBF = 3;
const size_t RBB = 4;
const size_t RTB = 5;
const size_t LTB = 6;
const size_t LBB = 7;

void addTriangle(Buffer &buffer, std::array<VertexIndex, 8> &indices, size_t v1, size_t v2, size_t v3)
{
    addTriangle(buffer, indices[v1], indices[v2], indices[v3]);
}

void addCube(Buffer &buffer, glm::mat4 transform)
{
    float vertices[] = {
        0.5f, -0.5f, 0.5f,   // RBF
        0.5f, 0.5f, 0.5f,    // RTF
        -0.5f, 0.5f, 0.5f,   // LTF
        -0.5f, -0.5f, 0.5f,  // LBF
        0.5f, -0.5f, -0.5f,  // RBB
        0.5f, 0.5f, -0.5f,   // RTB
        -0.5f, 0.5f, -0.5f,  // LTB
        -0.5f, -0.5f, -0.5f, // LBB
    };

    std::array<VertexIndex, 8> indices;

    glm::vec4 v;
    for (auto i = 0; i < 8; i++) {
        v = glm::vec4(vertices[(i*3)], vertices[(i*3)+1], vertices[(i*3)+2], 1.0f) * transform;
        indices[i] = addVertex(buffer, {{v[0], v[1], v[2]}, {255, 255, 255, 255}});
    }

    addTriangle(buffer, indices, RBF, RTF, LTF);
    addTriangle(buffer, indices, RBF, LTF, LBF);
    addTriangle(buffer, indices, RBB, RTB, RTF);
    addTriangle(buffer, indices, RBB, RTF, RBF);
    addTriangle(buffer, indices, RTF, RTB, LTB);
    addTriangle(buffer, indices, RTF, LTB, LTF);
    addTriangle(buffer, indices, LTB, RTB, RBB);
    addTriangle(buffer, indices, LBB, LTB, RBB);
    addTriangle(buffer, indices, LTF, LTB, LBB);
    addTriangle(buffer, indices, LBF, LTF, LBB);
    addTriangle(buffer, indices, LBB, RBB, RBF);
    addTriangle(buffer, indices, LBF, LBB, RBF);
}
