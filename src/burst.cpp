#include "burst.h"
#include "ofGraphics.h"
#include "ofMatrix4x4.h"
#include "of3dPrimitives.h"

Burst::Burst(ofxVboAppender &vboAppender)
    : vboAppender(vboAppender)
{

}

RuleHandle Burst::add_rule(std::string name)
{
    rules.push_back({name, {}});
    return RuleHandle(*this, rules.size() - 1);
}

void Burst::translateX(size_t ruleIndex, float delta)
{
    rules[ruleIndex].actions.push_back([this, delta] {
        ofMatrix4x4 m;
        m.makeTranslationMatrix(delta, 0.0, 0.0);
        this->transformationMatrix *= m;
    });
}

void Burst::drawBox(size_t ruleIndex)
{
    rules[ruleIndex].actions.push_back([this] {
        ofLogNotice() << "drawBox";
        vboAppender.append(ofBoxPrimitive(10, 10, 10).getMesh(), {1, 1, 1, 1}, this->transformationMatrix);
    });
}

void Burst::run()
{
    rules[0].actions[0]();
    rules[0].actions[1]();
    rules[0].actions[2]();
    rules[0].actions[3]();
    rules[0].actions[4]();
}
    
RuleHandle::RuleHandle(Burst &burst, size_t ruleIndex)
    : burst(burst), ruleIndex(ruleIndex)
{

}

RuleHandle RuleHandle::translateX(float delta)
{
    burst.translateX(ruleIndex, delta);
    return *this;
}

RuleHandle RuleHandle::drawBox()
{
    burst.drawBox(ruleIndex);
    return *this;
}
