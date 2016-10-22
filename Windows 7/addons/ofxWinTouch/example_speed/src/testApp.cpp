#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	map<int,ofTouchEventArgs>::iterator iter;
	for ( iter=touches.begin(); iter != touches.end(); ++iter ){
		ofTouchEventArgs touch = iter->second;
		float velocity = sqrt(touch.xspeed * touch.xspeed + touch.yspeed * touch.yspeed);
		float brightness = velocity * 5;
		float radius = 20 + velocity * 0.5;
		ofSetColor(255, velocity, velocity);
		ofCircle(iter->second.x, iter->second.y, radius);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	touches.insert(pair<int,ofTouchEventArgs>(touch.id,touch));
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	touch.xspeed = touch.x - touches[touch.id].x;
	touch.yspeed = touch.y - touches[touch.id].y;
	touches[touch.id] = touch;
}
		
//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	touches.erase(touch.id);
}
		
//--------------------------------------------------------------
void testApp::twoFingerTap(){
}

//--------------------------------------------------------------
void testApp::twoFingerZoom(double dZoomFactor,const LONG lZx,const LONG lZy) {
}