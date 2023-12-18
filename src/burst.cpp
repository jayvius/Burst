#include "burst.h"
#include "ofGraphics.h"
#include "ofMatrix4x4.h"
#include "of3dPrimitives.h"
#include "scanner.h"
#include "parser.h"

Burst::Burst(ofxVboAppender &vboAppender, std::mutex &updateMutex)
    : vboAppender(vboAppender), updateMutex(updateMutex)
{

}

void Burst::load(std::string src)
{
    Scanner scanner(src);
    //while (std::optional<Token> t = scanner.next()) {
    //    printf("%u %s\n", t->type, t->lexeme.c_str());
    //}
    rules = parse(scanner);
    // R0 maxdepth 50 : R1 ty 20.0 R0
    //rules.ruleTable.push_back({"R0", {5, 1, 2, 0, 0, 160, 65, 5, 0}, 1, 50});
    // R1 maxdepth 50 : R2 tz 20.0 R1
    //rules.ruleTable.push_back({"R1", {5, 2, 3, 0, 0, 160, 65, 5, 1}, 0, 50});
    // R2 maxdepth 50 : box tx 20.0 R2
    //rules.ruleTable.push_back({"R2", {4, 1, 0, 0, 160, 65, 5, 2}, 0, 50});
}

void Burst::run()
{
    size_t rule = 0;
    size_t ruleIndex = 0;
    //while (rules.ruleTable[rule].currentDepth < rules.ruleTable[rule].maxDepth) {
    while (ruleStack.size() > 0 || ruleIndex < rules[rule].byteCode.size()) {
        //printf("%u %u %u %u %u\n", rules.ruleTable[rule].currentDepth, rules.ruleTable[rule].maxDepth, rule, ruleIndex, rules.ruleTable[rule].byteCode[ruleIndex]);
        if (ruleIndex >= rules[rule].byteCode.size()) {
            rules[rule].currentDepth--;
            rule = ruleStack.back();
            ruleStack.pop_back();
            ruleIndex = ruleIndexStack.back();
            ruleIndexStack.pop_back();
            this->transformationMatrix = this->transformationStack.back();
            this->transformationStack.pop_back();
            //printf("JNB: %u %u\n", rule, ruleIndex);
            continue;
        }
        if (rules[rule].byteCode[ruleIndex] == 1) {
            ruleIndex++;
            float delta;
            memcpy(&delta, rules[rule].byteCode.data() + ruleIndex, 4);
            ruleIndex += 4;
            //printf("tx %f\n", delta);
            this->translateX(delta);
        }
        else if (rules[rule].byteCode[ruleIndex] == 2) {
            ruleIndex++;
            float delta;
            memcpy(&delta, rules[rule].byteCode.data() + ruleIndex, 4);
            ruleIndex += 4;
            //printf("ty %f\n", delta);
            this->translateY(delta);
        }
        else if (rules[rule].byteCode[ruleIndex] == 3) {
            ruleIndex++;
            float delta;
            memcpy(&delta, rules[rule].byteCode.data() + ruleIndex, 4);
            ruleIndex += 4;
            //printf("tz %f\n", delta);
            this->translateZ(delta);
        }
        else if (rules[rule].byteCode[ruleIndex] == 4) {
            ruleIndex++;
            //printf("box\n");
            this->drawBox();
        }
        else if (rules[rule].byteCode[ruleIndex] == 5) {
            ruleIndex++;
            size_t nextRule = rules[rule].byteCode[ruleIndex++];
            if (rules[nextRule].currentDepth < rules[nextRule].maxDepth) {
                ruleStack.push_back(rule);
                ruleIndexStack.push_back(ruleIndex);
                rule = nextRule;
                rules[rule].currentDepth++;
                ruleIndex = 0;
                this->transformationStack.push_back(this->transformationMatrix);
                //printf("rule %u\n", rule);
            }
        }
        else if (rules[rule].byteCode[ruleIndex] == static_cast<uint8_t>(OpCode::exit)) {
            return;
        }
        else {
            uint8_t temp = rules[rule].byteCode[ruleIndex];
            fprintf(stderr, "ERROR: invalid command: %u\n", temp);
            exit(1);
        }
    }
    /*
    std::vector<uint8_t> buffer(5, 0);
    while (true) {
        size_t count = streamer->get(buffer.data(), 1);
        printf("count: %u\n", count);
        for (int i = 0; i < count; i++) {
            printf("data: %x ", buffer[i]);
        }
        printf("\n");
        if (buffer[0] == 1) {
            count = streamer->get(buffer.data(), 4);
            printf("count: %u\n", count);
            for (int i = 0; i < count; i++) {
                printf("data: %x ", buffer[i]);
            }
            float delta;
            memcpy(&delta, buffer.data(), 4);
            printf("tx: %f\n", delta);
            this->translateX(delta);
        }
        else if (buffer[0] == 4) {
            printf("box\n");
            this->drawBox();
        }
    }*/
}

void Burst::translateX(float delta)
{
    ofMatrix4x4 m;
    m.makeTranslationMatrix(delta, 0.0, 0.0);
    this->transformationMatrix *= m;
}

void Burst::translateY(float delta)
{
    ofMatrix4x4 m;
    m.makeTranslationMatrix(0.0, delta, 0.0);
    this->transformationMatrix *= m;
}

void Burst::translateZ(float delta)
{
    ofMatrix4x4 m;
    m.makeTranslationMatrix(0.0, 0.0, delta);
    this->transformationMatrix *= m;
}

void Burst::drawBox()
{
    std::lock_guard<std::mutex> guard(this->updateMutex);
    vboAppender.append(ofBoxPrimitive(10, 10, 10, 1, 1, 1).getMesh(), {1, 1, 1, 1}, this->transformationMatrix);
}
