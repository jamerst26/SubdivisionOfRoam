#include "testApp.h"

void testApp::setup(){
	camera.initGrabber(camWidth, camHeight);
	blur.setup(camWidth, camHeight);
}

void testApp::update(){
	camera.grabFrame();
}

void testApp::draw(){	
	blur.setRadius(ofMap(mouseX, 0, ofGetWidth(), 0, 50));
	blur.setPasses(ofMap(mouseY, 0, ofGetHeight(), 0, 50));
	
	blur.begin();
	camera.draw(0, 0);
	blur.end();
	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}
