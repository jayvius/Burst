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
    return add_rule(name, 100000);
}

RuleHandle Burst::add_rule(std::string name, size_t maxDepth)
{
    rules.push_back({name, {}, 0, maxDepth});
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

void Burst::callRule(size_t ruleIndex, std::string ruleName)
{
    rules[ruleIndex].actions.push_back([this, ruleName] {
        for (auto &r: this->rules) {
            if (r.name == ruleName && r.callCount < r.maxDepth) {
                this->actionStack.push_back(this->actionIndex);
                this->actionIndex = 0;
                this->ruleStack.push_back(this->ruleIndex);
                this->ruleIndex = 0;
                r.callCount++;
                break;
            }
        }
    });
}

void Burst::run()
{
    ruleIndex = 0;
    actionIndex = 0;
    while (true) {
        while (actionIndex < rules[ruleIndex].actions.size()) {
            auto action = rules[ruleIndex].actions[actionIndex];
            actionIndex++;
            action();
        }

        if (actionStack.size() == 0 || ruleStack.size() == 0)
            return;

        actionIndex = actionStack.back();
        actionStack.pop_back();
        ruleIndex = ruleStack.back();
        ruleStack.pop_back();
    }
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

RuleHandle RuleHandle::callRule(std::string ruleName)
{
    burst.callRule(ruleIndex, ruleName);
    return *this;
}
