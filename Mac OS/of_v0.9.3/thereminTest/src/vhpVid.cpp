#include "vhpVid.h"

//--------------------------------------------------------------
vhpVid::vhpVid(): alpha(0), alpha_increment(15){

}

//--------------------------------------------------------------
void vhpVid::setup(){
    backVideo.load(backVideoList[0]);
    frontVideo.load(frontVideoList[0]);
    width = backVideo.getWidth();
    height = backVideo.getHeight();
    fbo.allocate(width, height, GL_RGBA);
    bufferTex.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    setPlay();
    shaderMixture.load("shadersGL2/mixture");
}

//--------------------------------------------------------------
void vhpVid::setup(string _file){
    backVideo.load(_file);
    width = backVideo.getWidth();
    height = backVideo.getHeight();
    fbo.allocate(width, height, GL_RGBA);
    bufferTex.allocate(width, height, GL_RGB);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    setPlay();
    shaderMixture.load("shadersGL2/mixture");
}

//--------------------------------------------------------------
void vhpVid::addVideos(ofxXmlSettings& _videoList, string _backTag, string _frontTag){
    int n = _videoList.getNumTags(_backTag +":VIDEO");
    if(n > 0){
        _videoList.pushTag(_backTag, n-1);
        int num = _videoList.getNumTags("VIDEO");
        cout << num << " videos in BACKGROUND" << endl;
        if(num > 0){
            for(int i = 0; i < num; i++){
                cout << _videoList.getValue("VIDEO", "", i) << endl;
                backVideoList.push_back(_videoList.getValue("VIDEO", "", i));
            }
        }
        _videoList.popTag();
    }
    n = _videoList.getNumTags(_frontTag +":VIDEO");
    if(n > 0){
        _videoList.pushTag(_frontTag, n-1);
        int num = _videoList.getNumTags("VIDEO");
        cout << num << " videos in BACKGROUND" << endl;
        if(num > 0){
            for(int i = 0; i < num; i++){
                cout << _videoList.getValue("VIDEO", "", i) << endl;
                frontVideoList.push_back(_videoList.getValue("VIDEO", "", i));
            }
        }
        _videoList.popTag();
    }
}

//--------------------------------------------------------------
void vhpVid::update(){
    (*this.*currentUpdate)();
}

//--------------------------------------------------------------
void vhpVid::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

//--------------------------------------------------------------
void vhpVid::backPlay(){
    backVideo.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    backVideo.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

//--------------------------------------------------------------
void vhpVid::frontPlay(){
    frontVideo.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    frontVideo.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

//--------------------------------------------------------------
void vhpVid::frontFadeIn(){
    backVideo.update();
    frontVideo.update();
    bufferTex.loadData(backVideo.getPixels(), width, height, GL_RGB);
    ofPushStyle();
    fbo.begin();
    shaderMixture.begin();
    shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    shaderMixture.setUniform1f("mixture", alpha/255.0);
    ofSetColor(255, 255, 255);
    frontVideo.draw(0, 0);
    shaderMixture.end();
    fbo.end();
    ofPopStyle();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        currentUpdate = &vhpVid::frontPlay;
        backVideo.stop();
    }
}

//--------------------------------------------------------------
void vhpVid::frontFadeout(){
    backVideo.update();
    frontVideo.update();
    bufferTex.loadData(backVideo.getPixels(), width, height, GL_RGB);
    ofPushStyle();
    fbo.begin();
    shaderMixture.begin();
    shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    shaderMixture.setUniform1f("mixture", alpha/255.0);
    ofSetColor(255, 255, 255);
    frontVideo.draw(0, 0);
    shaderMixture.end();
    fbo.end();
    ofPopStyle();
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0;
        currentUpdate = &vhpVid::backPlay;
        frontVideo.stop();
    }
}

//--------------------------------------------------------------
void vhpVid::pause(){
    
}

//--------------------------------------------------------------
void vhpVid::loop(float _pos){
    backVideo.setPosition(_pos);
}

//--------------------------------------------------------------
void vhpVid::setPlay(){
    backVideo.play();
    currentUpdate = &vhpVid::backPlay;
}

//--------------------------------------------------------------
void vhpVid::setPause(){
    backVideo.stop();
    currentUpdate = &vhpVid::pause;
}

//--------------------------------------------------------------
void vhpVid::setFrontFadeIn(){
    ofEnableAlphaBlending();
    frontVideo.play();
    currentUpdate = &vhpVid::frontFadeIn;
}


//--------------------------------------------------------------
float vhpVid::getPosition(){
    return backVideo.getPosition();
}