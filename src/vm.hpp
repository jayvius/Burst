#ifndef VM_HPP
#define VM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "buffer.hpp"
#include "rules.h"

struct VM
{
    size_t currentRuleIndex;
    glm::mat4 transformation;
};

void run(VM &vm, std::string src, Buffer &buffer);

#endif
