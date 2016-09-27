#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    thereminTourette.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    thereminTourette.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    thereminTourette.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    thereminTourette.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //thereminTourette.mousePressed(x*3, y*3, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //thereminTourette.mouseReleased(x*3, y*3, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}