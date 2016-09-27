#include "vhpScreenSaver.h"

//--------------------------------------------------------------
vhpScreenSaver::vhpScreenSaver(){

}

//--------------------------------------------------------------
vhpScreenSaver::~vhpScreenSaver(){
    if(registerEvents) {
        ofUnregisterMouseEvents(this); // disable litening to mous events.
        registerEvents = false;
    }
}

//--------------------------------------------------------------
void vhpScreenSaver::setup(string _file){
    video.load(_file);
    width = video.getWidth();
    height = video.getHeight();
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
}

//--------------------------------------------------------------
void vhpScreenSaver::setId(int _id, int _target){
    gameId = _id;
    gameTarget = _target;
}

//--------------------------------------------------------------
void vhpScreenSaver::init(){
    if(!registerEvents) {
        ofRegisterMouseEvents(this); // this will enable our circle class to listen to the mouse events.
        registerEvents = true;
    }
    setPlay();
}

//--------------------------------------------------------------
void vhpScreenSaver::stop(){
    if(registerEvents) {
        ofUnregisterMouseEvents(this); // disable litening to mous events.
        registerEvents = false;
    }
    setPause();
}

//--------------------------------------------------------------
void vhpScreenSaver::addVideos(ofxXmlSettings& _videoList, string _videoTag){
    int n = _videoList.getNumTags(_videoTag +":VIDEO");
    if(n > 0){
        _videoList.pushTag(_videoTag, n-1);
        int num = _videoList.getNumTags("VIDEO");
        cout << num << " videos in " << _videoTag << " " << _videoList.getValue("VIDEO", "", 0) << endl;
        if(num > 0) setup(_videoList.getValue("VIDEO", "", 0));
        _videoList.popTag();
    }
}

//--------------------------------------------------------------
void vhpScreenSaver::update(){
    (*this.*currentUpdate)();
}

//--------------------------------------------------------------
void vhpScreenSaver::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

//--------------------------------------------------------------
void vhpScreenSaver::play(){
    video.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    video.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

//--------------------------------------------------------------
void vhpScreenSaver::pause(){
    
}

//--------------------------------------------------------------
void vhpScreenSaver::loop(float _pos){
    video.setPosition(_pos);
}

//--------------------------------------------------------------
void vhpScreenSaver::setPlay(){
    video.play();
    currentUpdate = &vhpScreenSaver::play;
}

//--------------------------------------------------------------
void vhpScreenSaver::setPause(){
    video.stop();
    currentUpdate = &vhpScreenSaver::pause;
}

//--------------------------------------------------------------
float vhpScreenSaver::getPosition(){
    return video.getPosition();
}

// EVENTS ------------------------------------------------------
void vhpScreenSaver::mouseMoved(ofMouseEventArgs & _args){}
void vhpScreenSaver::mouseDragged(ofMouseEventArgs & _args){}
void vhpScreenSaver::mousePressed(ofMouseEventArgs & _args){}
void vhpScreenSaver::mouseReleased(ofMouseEventArgs & _args){
    cout << "Button active!" << endl;
    ofNotifyEvent(onClick, gameTarget);
}
void vhpScreenSaver::mouseScrolled(ofMouseEventArgs & _args){}
void vhpScreenSaver::mouseEntered(ofMouseEventArgs & _args){}
void vhpScreenSaver::mouseExited(ofMouseEventArgs & _args){}

ofEvent <int> vhpScreenSaver::onClick;
