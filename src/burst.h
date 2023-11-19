#ifndef BURST_H
#define BURST_H

#include <string>
#include <variant>
#include <vector>
#include <memory>
#include <mutex>
#include "ofxVboAppender.h"
#include "server.h"


struct Rule
{
    std::vector<uint8_t> byteCode;
    size_t currentDepth;
    size_t maxDepth;
};

struct Rules
{
    std::vector<Rule> ruleTable;
    std::vector<size_t> ruleStack;
    std::vector<size_t> ruleIndexStack;
};

class Burst
{
public:
    Burst(ofxVboAppender &vboAppender, std::mutex &updateMutex);
    void run(std::unique_ptr<ByteCodeStreamer> streamer);

private:
    void translateX(float delta);
    void translateY(float delta);
    void translateZ(float delta);
    void drawBox();

    ofxVboAppender &vboAppender;
    std::mutex &updateMutex;
    ofMatrix4x4 transformationMatrix;
    std::vector<ofMatrix4x4> transformationStack;
};

#endif
