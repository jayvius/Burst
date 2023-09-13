#pragma once

#include "ofVbo.h"
#include "ofBufferObject.h"
#include "ofVectorMath.h"
#include "ofMesh.h"

class ofxVboAppender
{
public:
	ofxVboAppender(uint32_t maxVertexCount = 100000);
	void append(ofVec3f position, ofVec4f color, ofVec3f normal);
	void append(ofMesh &mesh, ofVec4f color);
	void append(ofMesh &mesh, ofVec4f color, ofMatrix4x4 &transformMatrix);
	void draw();

private:
	ofVbo vbo;
	ofBufferObject positionBuffer;
	ofBufferObject colorBuffer;
	ofBufferObject normalBuffer;
	uint32_t vertexCount;
	uint32_t maxVertexCount;
};
