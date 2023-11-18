#ifndef BURST_H
#define BURST_H

#include <string>
#include <variant>
#include <vector>
#include <memory>
#include "ofxVboAppender.h"
#include "server.h"

class Burst
{
public:
    Burst(ofxVboAppender &vboAppender);
    void run(std::unique_ptr<ByteCodeStreamer> streamer);

private:
    void translateX(float delta);
    void translateY(float delta);
    void translateZ(float delta);
    void drawBox();

    ofxVboAppender &vboAppender;
    ofMatrix4x4 transformationMatrix;
    std::vector<ofMatrix4x4> transformationStack;
};

#endif
