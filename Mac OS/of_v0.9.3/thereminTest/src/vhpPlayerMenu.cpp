#include "vhpPlayerMenu.h"

//--------------------------------------------------------------
vhpPlayerMenu::vhpPlayerMenu():scale(1.0),state(MENU){

}

//--------------------------------------------------------------
vhpPlayerMenu::~vhpPlayerMenu(){
    if(registerEvents) {
        ofUnregisterMouseEvents(this); // disable litening to mous events.
        registerEvents = false;
    }
    videoController.stop();
}

//--------------------------------------------------------------
void vhpPlayerMenu::setup(string _file){
    videoController.setup(&video, 0.0, 9.0/57.0);
    video.load(_file);
    width = video.getWidth();
    height = video.getHeight();
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
}

//--------------------------------------------------------------
void vhpPlayerMenu::setId(int _id, int _target){
    gameId = _id;
    gameTarget = _target;
}

//--------------------------------------------------------------
void vhpPlayerMenu::init(){
    if(!registerEvents) {
        ofRegisterMouseEvents(this); // this will enable our circle class to listen to the mouse events.
        registerEvents = true;
    }
    setPlay();
    videoController.start();
}

//--------------------------------------------------------------
void vhpPlayerMenu::addVideos(ofxXmlSettings& _videoList, string _videoTag){
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
void vhpPlayerMenu::update(){
    (*this.*currentUpdate)();
}

//--------------------------------------------------------------
void vhpPlayerMenu::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

//--------------------------------------------------------------
void vhpPlayerMenu::play(){
    video.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    video.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

//--------------------------------------------------------------
void vhpPlayerMenu::pause(){
    
}

//--------------------------------------------------------------
void vhpPlayerMenu::loop(float _pos){
    video.setPosition(_pos);
}

//--------------------------------------------------------------
void vhpPlayerMenu::setPlay(){
    video.play();
    currentUpdate = &vhpPlayerMenu::play;
}

//--------------------------------------------------------------
void vhpPlayerMenu::setPause(){
    video.stop();
    currentUpdate = &vhpPlayerMenu::pause;
}

//--------------------------------------------------------------
float vhpPlayerMenu::getPosition(){
    return video.getPosition();
}

// EVENTS ------------------------------------------------------
void vhpPlayerMenu::mouseMoved(ofMouseEventArgs & _args){}
void vhpPlayerMenu::mouseDragged(ofMouseEventArgs & _args){}
void vhpPlayerMenu::mousePressed(ofMouseEventArgs & _args){}
void vhpPlayerMenu::mouseReleased(ofMouseEventArgs & _args){
    cout << "Menu active!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    // min x: 520, min y: 182 408 538 630 160
    cout << "mouse x: " << _args.x << " mouse y: " << _args.y << endl;
    float y = _args.y*3/scale;
    if (y>=520) {
        float x =  _args.x*3/scale;
        if ((x>=182)&&(x<=1760)) {
            if (x<=592) {
                cout << "1 xogador" << endl;
                switch (state) {
                    case MENU:
                        video.setPosition(10.0/57.0);
                        videoController.reset(12.0/57.0, 16.0/57.0);
                        break;
                    case TWOPLAYERS:
                        video.setPosition(29.0/57.0);
                        videoController.fadeReset(10.0/57.0, 30.0/57.0, 12.0/57.0, 16.0/57.0);
                        break;
                    case FOURPLAYERS:
                        video.setPosition(41.0/57.0);
                        videoController.fadeReset(10.0/57.0, 42.0/57.0, 12.0/57.0, 16.0/57.0);
                        break;
                }
                state = ONEPLAYER;
            } else if (x<=1130) {
                cout << "2 xogadores" << endl;
                switch (state) {
                    case MENU:
                        video.setPosition(22.0/57.0);
                        videoController.reset(24.0/57.0, 28.0/57.0);
                        break;
                    case ONEPLAYER:
                        video.setPosition(17.0/57.0);
                        videoController.fadeReset(22.0/57.0, 18.0/57.0, 24.0/57.0, 28.0/57.0);
                        break;
                    case FOURPLAYERS:
                        video.setPosition(41.0/57.0);
                        videoController.fadeReset(22.0/57.0, 42.0/57.0, 24.0/57.0, 28.0/57.0);
                        break;
                }
                state = TWOPLAYERS;
            } else {
                cout << "4 xogadores" << endl;
                switch (state) {
                    case MENU:
                        video.setPosition(34.0/57.0);
                        videoController.reset(36.0/57.0, 40.0/57.0);
                        break;
                    case ONEPLAYER:
                        video.setPosition(17.0/57.0);
                        videoController.fadeReset(34.0/57.0, 18.0/57.0, 36.0/57.0, 40.0/57.0);
                        break;
                    case TWOPLAYERS:
                        video.setPosition(29.0/57.0);
                        videoController.fadeReset(34.0/57.0, 30.0/57.0, 36.0/57.0, 40.0/57.0);
                        break;
                }
                state = FOURPLAYERS;
            }
        } else {
            cout << "fora" << endl;
            switch (state) {
                case ONEPLAYER:
                    video.setPosition(17.0/57.0);
                    videoController.fadeReset(0.0, 18.0/57.0, 0.0, 9.0/57.0);
                    break;
                case TWOPLAYERS:
                    video.setPosition(29.0/57.0);
                    videoController.fadeReset(0.0, 30.0/57.0, 0.0, 9.0/57.0);
                    break;
                case FOURPLAYERS:
                    video.setPosition(41.0/57.0);
                    videoController.fadeReset(0.0, 42.0/57.0, 0.0, 9.0/57.0);
                    break;
            }
            state = MENU;
        }
    } else {
        if (state != MENU) {
            cout << "fora" << endl;
            switch (state) {
                case ONEPLAYER:
                    video.setPosition(17.0/57.0);
                    videoController.fadeReset(0.0, 18.0/57.0, 0.0, 9.0/57.0);
                    break;
                case TWOPLAYERS:
                    video.setPosition(29.0/57.0);
                    videoController.fadeReset(0.0, 30.0/57.0, 0.0, 9.0/57.0);
                    break;
                case FOURPLAYERS:
                    video.setPosition(41.0/57.0);
                    videoController.fadeReset(0.0, 42.0/57.0, 0.0, 9.0/57.0);
                    break;
            }
            state = MENU;
        }
    }
}
void vhpPlayerMenu::mouseScrolled(ofMouseEventArgs & _args){}
void vhpPlayerMenu::mouseEntered(ofMouseEventArgs & _args){}
void vhpPlayerMenu::mouseExited(ofMouseEventArgs & _args){}

//ofEvent <int> vhpPlayerMenu::onClick;
