#ifndef OPCODES_H
#define OPCODES_H

enum class OpCode
{
    exit = 0,
    translateX,
    translateY,
    translateZ,
    rotateX,
    rotateY,
    rotateZ,
    scale,
    drawBox,
    callRule,
    callRandomRule,
};

#endif
