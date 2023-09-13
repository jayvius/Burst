#include "ofApp.h"
#include <glm/gtx/string_cast.hpp>

ofApp::ofApp()
    : dampen(0.4), burst(vboAppender)
{

}

void ofApp::setup()
{
	ofEnableDepthTest();
    ofBackground(0);
    ofEnableAntiAliasing();
    ofEnableLighting();

    light1.enable();
    light1.setDirectional();
    light1.setDiffuseColor({0.7, 0.7, 0.7});
    light1.setOrientation({0.0, 180.0, 0.0});

    light2.enable();
    light2.setDirectional();
    light2.setDiffuseColor({0.5, 0.5, 0.5});
    light2.setOrientation({0.0, -90.0, 0.0});

    light3.enable();
    light3.setDirectional();
    light3.setDiffuseColor({0.9, 0.9, 0.9});
    light3.setOrientation({90.0, 0.0, 0.0});

    camera.setPosition({200.0, 200.0, 200.0});
    camera.setTarget({0.0, 0.0, 0.0});
    camera.removeAllInteractions();
    camera.setFarClip(100000.0);
    
    auto rule1 = burst.add_rule("R1");
    rule1.drawBox().translateX(20.0).drawBox().translateX(20.0).drawBox();
    burst.run();
}

void ofApp::update()
{
    // Slow down model rotation
	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f/5000.f, 1.f/5.f);
	xSpeed /= 1.f + deltaTime;
	ySpeed /= 1.f + deltaTime;

    // Update model rotation
	glm::quat yRot = glm::angleAxis(ofDegToRad(xSpeed), glm::vec3(0,1,0));
	glm::quat xRot = glm::angleAxis(ofDegToRad(ySpeed), glm::vec3(0.7071,0,-0.7071));
	modelRotation = yRot * xRot * modelRotation;

    auto box = ofBoxPrimitive(10, 10, 10, 1, 1, 1);
    ofMatrix4x4 transformMatrix;
    transformMatrix.makeTranslationMatrix({ofRandom(-150, 150), ofRandom(-150, 150), ofRandom(-150, 150)});
    ofMatrix4x4 rotateMatrix;
    rotateMatrix.makeRotationMatrix(ofRandom(89), {0, 1, 0});
    //transformMatrix = rotateMatrix * transformMatrix;
    //vboAppender.append(box.getMesh(), {1, 1, 1, 1}, transformMatrix);
}

void ofApp::draw()
{
    camera.begin();
	ofPushMatrix();

	ofMultMatrix(glm::mat4(modelRotation));

    ofDrawAxis(100);
    material.begin();

    vboAppender.draw();

    material.end();
    camera.end();
	
    ofPopMatrix();

	ofPopMatrix();
}

void ofApp::mouseDragged(int x, int y, int button)
{
    // Calculate model rotation based on mouse drag
	glm::vec2 mouse(x, y);
	xSpeed = ofLerp(xSpeed, (x - lastMousePosition.x) * dampen, 0.1);
	ySpeed = ofLerp(ySpeed, (y - lastMousePosition.y) * dampen, 0.1);
	lastMousePosition = mouse;
    printf("xSpeed=%f ySpeed=%f\n", xSpeed, ySpeed);
}

void ofApp::mousePressed(int x, int y, int button)
{
    // Initialize mouse click position
	lastMousePosition = glm::vec2(x, y);
}
