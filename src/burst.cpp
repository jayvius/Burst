#include "burst.h"
#include "ofGraphics.h"
#include "ofMatrix4x4.h"
#include "of3dPrimitives.h"

Burst::Burst(ofxVboAppender &vboAppender, std::mutex &updateMutex)
    : vboAppender(vboAppender), updateMutex(updateMutex)
{

}

void Burst::run(std::unique_ptr<ByteCodeStreamer> streamer)
{
    std::vector<uint8_t> buffer(3, 0);
    while (true) {
        size_t count = streamer->get(buffer.data(), buffer.size());
        printf("count=%u\n", count);
        for (int i = 0; i < count; i++) {
            printf("%u\n", buffer[i]);
        }
    }
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
