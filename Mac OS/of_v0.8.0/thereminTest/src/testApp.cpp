#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFullscreen(true);
    thereminTourette.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    thereminTourette.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    thereminTourette.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    thereminTourette.keyReleased(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    thereminTourette.touchPressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    thereminTourette.touchReleased(x, y, button);
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
