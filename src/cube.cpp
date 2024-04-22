#include "cube.hpp"
#include <array>
#include <fmt/core.h>


void addCube(Buffer &buffer, glm::mat4 transform)
{
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
    unsigned char a = 255;
    VertexIndex v0, v1, v2;
    // Generated by "python scripts/convert_obj.py data/cube.obj"
    v0 = addVertex(buffer, {{0.5, 0.5, 0.5}, {-0.0, 1.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{-0.5, 0.5, -0.5}, {-0.0, 1.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{-0.5, 0.5, 0.5}, {-0.0, 1.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{-0.5, 0.5, -0.5}, {-0.0, -0.0, -1.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{0.5, -0.5, -0.5}, {-0.0, -0.0, -1.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{-0.5, -0.5, -0.5}, {-0.0, -0.0, -1.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{0.5, 0.5, -0.5}, {1.0, -0.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{0.5, -0.5, 0.5}, {1.0, -0.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{0.5, -0.5, -0.5}, {1.0, -0.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{-0.5, -0.5, 0.5}, {-0.0, -1.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{0.5, -0.5, -0.5}, {-0.0, -1.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{0.5, -0.5, 0.5}, {-0.0, -1.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{-0.5, 0.5, 0.5}, {-1.0, -0.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{-0.5, -0.5, -0.5}, {-1.0, -0.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{-0.5, -0.5, 0.5}, {-1.0, -0.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{0.5, 0.5, 0.5}, {-0.0, -0.0, 1.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{-0.5, -0.5, 0.5}, {-0.0, -0.0, 1.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{0.5, -0.5, 0.5}, {-0.0, -0.0, 1.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{0.5, 0.5, 0.5}, {-0.0, 1.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{0.5, 0.5, -0.5}, {-0.0, 1.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{-0.5, 0.5, -0.5}, {-0.0, 1.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{-0.5, 0.5, -0.5}, {-0.0, -0.0, -1.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{0.5, 0.5, -0.5}, {-0.0, -0.0, -1.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{0.5, -0.5, -0.5}, {-0.0, -0.0, -1.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{0.5, 0.5, -0.5}, {1.0, -0.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{0.5, 0.5, 0.5}, {1.0, -0.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{0.5, -0.5, 0.5}, {1.0, -0.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{-0.5, -0.5, 0.5}, {-0.0, -1.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{-0.5, -0.5, -0.5}, {-0.0, -1.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{0.5, -0.5, -0.5}, {-0.0, -1.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{-0.5, 0.5, 0.5}, {-1.0, -0.0, -0.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{-0.5, 0.5, -0.5}, {-1.0, -0.0, -0.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{-0.5, -0.5, -0.5}, {-1.0, -0.0, -0.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
    v0 = addVertex(buffer, {{0.5, 0.5, 0.5}, {-0.0, -0.0, 1.0}, {r, g, b, a}});
    v1 = addVertex(buffer, {{-0.5, 0.5, 0.5}, {-0.0, -0.0, 1.0}, {r, g, b, a}});
    v2 = addVertex(buffer, {{-0.5, -0.5, 0.5}, {-0.0, -0.0, 1.0}, {r, g, b, a}});
    addTriangle(buffer, v0, v1, v2);
}
