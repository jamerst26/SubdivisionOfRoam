#include "Hole.h"

//#define USE_JITTERS

int Hole::holeRadius;
float Hole::maxHoleAge;
bool Hole::useEllipses;
float Hole::deshake;

Hole::Hole() {
	birth = ofGetElapsedTimef();
}

void Hole::setup(ofxVec2f position) {
	img = HoleManager::randomHoleImage();
	this->position = position;
}

void Hole::update() {
	vector<ofxCvBlob>& blobs = testApp::contourFinder.blobs;
	position.interpolate(ContourMatcher::closestPoint(blobs, position), deshake);
}

void Hole::draw() {
	if(useEllipses) {
		ofSetColor(255);
		ofEllipse(position.x, position.y, holeRadius, holeRadius);
	} else {	
		//cout << "drawing hole at " << blobIndex << "/" << center << " " << position.x << "/" << position.y << endl;
		
		ofxCvBlob& blob = testApp::contourFinder.blobs[blobIndex];
		vector<ofPoint>& points = blob.pts;
		int start = ofClamp(center - holeRadius, 0, points.size());
		int stop = ofClamp(center + holeRadius, 0, points.size());
		
		if(start < stop) {
			ofFill();
			glColor3f(1, 1, 1);
			ofBeginShape();
			for(int i = start; i < stop; i++) {
				ofPoint& cur = points[i];
				ofVertex(cur.x, cur.y);
			}
			ofEndShape();

			ofxVec2f startPoint(points[start]);
			ofxVec2f stopPoint(points[stop - 1]);
			ofxVec2f diff = stopPoint - startPoint;
			float angle = atan2f(diff.y, diff.x) - PI / 2;
			glPushMatrix();
			glTranslatef(startPoint.x, startPoint.y, 0);
			glRotatef(ofRadToDeg(angle), 0, 0, 1);
			float scale = diff.length() / img->getHeight();
			glScalef(scale, scale, 1);
			img->draw(0, 0);
			ofNoFill();
			if(testApp::debug) {
				ofRect(0, 0, img->getWidth(), img->getHeight());
			}
			glPopMatrix();
		}
	}
}

float Hole::distance(ofPoint& target) {
	return position.distance(target);
}

float Hole::getAge() {
	return ofGetElapsedTimef() - birth;
}