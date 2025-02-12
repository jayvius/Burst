#pragma once

#include <vector>
#include "GL/gl3w.h"
#include <glm/glm.hpp>

#include "types.hpp"
#include "buffer.hpp"

void addCube(Buffer &buffer, glm::mat4 &transform, glm::vec<4, unsigned char> &rgba);
