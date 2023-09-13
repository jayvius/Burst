#pragma once

#include "ofMain.h"
#include "ofxVboAppender.h"
#include "burst.h"

class ofApp : public ofBaseApp
{
public:
	ofApp();
	void setup();
	void update();
	void draw();
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);

private:
    ofEasyCam camera;
	ofMaterial material;
	ofLight light1;
	ofLight light2;
	ofLight light3;

    ofxVboAppender vboAppender;
    Burst burst;
		
    // Model rotation
    glm::quat modelRotation;

    // Last mouse position
    glm::vec2 lastMousePosition;

    // Model rotation deceleration
    float dampen;

    // Model rotation speed
    float xSpeed = 0.0;
    float ySpeed = 0.0;
};
