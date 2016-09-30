#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
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
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //thereminTourette.mousePressed(x*3, y*3, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    //thereminTourette.mouseReleased(x*3, y*3, button);
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
