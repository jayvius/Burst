#include "ofxVboAppender.h"
#include "ofLog.h"

const size_t positionSize = 3 * sizeof(float);
const size_t colorSize = 4 * sizeof(float);
const size_t normalSize = 3 * sizeof(float);

ofxVboAppender::ofxVboAppender(uint32_t maxVertexCount)
    : vertexCount(0), updatedVertexCount(0), maxVertexCount(maxVertexCount)
{
    positionBuffer.allocate(maxVertexCount * positionSize, GL_STATIC_DRAW);
    vbo.setVertexBuffer(positionBuffer, 3, positionSize);
    colorBuffer.allocate(maxVertexCount * colorSize, GL_STATIC_DRAW);
    vbo.setColorBuffer(colorBuffer, colorSize);
    normalBuffer.allocate(maxVertexCount * normalSize, GL_STATIC_DRAW);
    vbo.setNormalBuffer(normalBuffer, normalSize);
}

void ofxVboAppender::append(ofVec3f position, ofVec4f color, ofVec3f normal)
{
    if (vertexCount >= maxVertexCount) {
        ofLog(OF_LOG_WARNING, "vertex limit of %d has been reached.", maxVertexCount);
        return;
    }
    positionData.push_back(position);
    colorData.push_back(color);
    normalData.push_back(normal);

    vertexCount++;
}

void ofxVboAppender::append(ofMesh &mesh, ofVec4f color)
{
    mesh.flatNormals();
    auto vertices = mesh.getVertices();
    auto normals = mesh.getNormals();

    for (auto i = 0; i < vertices.size(); i++) {
        ofVec3f v = vertices[i];
        ofVec3f n = normals[i];
        append(v, {1, 1, 1, 1}, n);
    }
}

void ofxVboAppender::append(ofMesh &mesh, ofVec4f color, ofMatrix4x4 &transformMatrix)
{
    mesh.flatNormals();
    auto vertices = mesh.getVertices();
    auto normals = mesh.getNormals();
    
    auto normalMatrix = ofMatrix4x4::getTransposedOf(transformMatrix.getInverse());

    for (auto i = 0; i < vertices.size(); i++) {
        ofVec3f v = vertices[i];
        ofVec4f v1 = ofVec4f(v.x, v.y, v.z, 1.0) * transformMatrix;
        ofVec3f n = normals[i];
        ofVec3f n1 = ofVec4f(n.x, n.y, n.z, 1.0) * normalMatrix;
        append(v1, {1, 1, 1, 1}, n1.getNormalized());
    }
}

void ofxVboAppender::draw()
{
    if (vertexCount > updatedVertexCount) {
        positionBuffer.updateData(updatedVertexCount * positionSize, vertexCount * positionSize, positionData.data());
        colorBuffer.updateData(updatedVertexCount * colorSize, vertexCount * colorSize, colorData.data());
        normalBuffer.updateData(updatedVertexCount * normalSize, vertexCount * normalSize, normalData.data());
        updatedVertexCount = vertexCount;
        positionData.clear();
        colorData.clear();
        normalData.clear();
    }
    
    vbo.draw(GL_TRIANGLES, 0, vertexCount);
}
