#ifndef BURST_H
#define BURST_H

#include <string>
#include <variant>
#include <vector>
#include "ofxVboAppender.h"

struct Rule
{
    std::string name;
    std::vector<std::function<void()>> actions;
};

class RuleHandle;

class Burst
{
public:
    Burst(ofxVboAppender &vboAppender);
    RuleHandle add_rule(std::string name);
    void translateX(size_t ruleIndex, float delta);
    void drawBox(size_t ruleIndex);
    void run();

private:
    ofxVboAppender &vboAppender;
    ofMatrix4x4 transformationMatrix;
    std::vector<Rule> rules;
    size_t currentRule;
    size_t currentAction;
    std::vector<size_t> ruleStack;
    std::vector<size_t> actionStack;
};

class RuleHandle
{
public:
    RuleHandle(Burst &burst, size_t ruleIndex);
    RuleHandle translateX(float delta);
    RuleHandle drawBox();

private:
    Burst &burst;
    size_t ruleIndex;
};

/*
class RuleHandle
{
public:
    RuleHandle(size_t ruleIndex, Burst &burst) : ruleIndex(ruleIndex), burst(burst) {}
    RuleHandle TranslateX(float delta) {burst.addTranslateX(delta); return *this}

private:
    size_t ruleIndex;
    Burst &burst;
};
*/

#endif
