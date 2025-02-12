#pragma once

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
    scaleX,
    scaleY,
    scaleZ,
    drawBox,
    callRule,
    callRandomRule,
};
