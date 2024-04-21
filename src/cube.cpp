#include "cube.hpp"
#include <array>

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"

void addCube(Buffer &buffer, glm::mat4 transform)
{
    par_shapes_mesh *cube = par_shapes_create_cube();
    for (auto i = 0; i < cube->ntriangles * 3; i += 3) {
        float x, y, z;
        x = cube->points[3*cube->triangles[i]] - 0.5;
        y = cube->points[3*cube->triangles[i]+1] - 0.5;
        z = cube->points[3*cube->triangles[i]+2] - 0.5;
        VertexIndex v0 = addVertex(buffer, {{x, y, z}, {255, 255, 255, 255}});
        x = cube->points[3*cube->triangles[i+1]] - 0.5;
        y = cube->points[3*cube->triangles[i+1]+1] - 0.5;
        z = cube->points[3*cube->triangles[i+1]+2] - 0.5;
        VertexIndex v1 = addVertex(buffer, {{x, y, z}, {255, 255, 255, 255}});
        x = cube->points[3*cube->triangles[i+2]] - 0.5;
        y = cube->points[3*cube->triangles[i+2]+1] - 0.5;
        z = cube->points[3*cube->triangles[i+2]+2] - 0.5;
        VertexIndex v2 = addVertex(buffer, {{x, y, z}, {255, 255, 255, 255}});
        addTriangle(buffer, v0, v1, v2);
    }
    par_shapes_free_mesh(cube);
}
