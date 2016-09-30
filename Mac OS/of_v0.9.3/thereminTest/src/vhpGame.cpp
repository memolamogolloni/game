#include "vhpGame.h"

// Constructor --------------------------------------------------
vhpGame::vhpGame(){

}

// Destructor ---------------------------------------------------
vhpGame::~vhpGame(){
    if (state==SCREENSAVER) ofRemoveListener(vhpScreenSaver::onClick, this, &vhpGame::set);
    /*
    {
        ofRemoveListener(vhpButton::onActive, this, &vhpGame::set);
        myTest.stop();
    }
     */
}

//--------------------------------------------------------------
void vhpGame::setup(){
    
    width = 1920;
    height = 1080;
    scale = 1.0;
    fullScreen.allocate(width*3, height, GL_RGBA);
    fullScreen.begin();
    ofClear(255,255,255, 0);
    fullScreen.end();
    
    bufferTex.allocate(width, height, GL_RGB);
    shaderMixture.load("shadersGL2/mixture");
    alpha = 0;
    alpha_increment = 15;
    
    TTF.load("mono.ttf", 7);
    
    cout << "loading videoList.xml" << endl;
    if( videoList.loadFile("videoList.xml") ){
        cout << "videoList.xml loaded!" << endl;
    }else{
        cout << "unable to load videoList.xml check data/ folder" << endl;
    }
    
    logos.setup(videoList, "LOGOS", SCREENSAVER, PLAYERMENU);
    
    //xogadores.addVideos(videoList, "XOGADORES");
    xogadores.setup(&controlXogadores, videoList, "XOGADORES");
    
    xogadores.setId(PLAYERMENU, STANDBY);
    
    state = SCREENSAVER;
    set(state);
    
    
    /*
    
    tactil.setup();
    
    int n = videoList.getNumTags("BACKGROUND:VIDEO");
    if(n > 0){
        videoList.pushTag("BACKGROUND", n-1);
        int num = videoList.getNumTags("VIDEO");
        cout << num << " videos in BACKGROUND" << endl;
        if(num > 0){
            background.resize(num);
            for(int i = 0; i < num; i++){
                cout << videoList.getValue("VIDEO", "", i) << endl;
                background[i].setup(videoList.getValue("VIDEO", "", i));
            }
        }
        videoList.popTag();
    }
   
    
    myTest.setup(&tactil, 0.0, 30.0/87.0);
    myTest.start();
     start.setRadius(50);
     start.setPosition(width/2, height/2);
     
    */
    
    /*
     n = videoList.getNumTags("WINDOWA:VIDEO");
     if(n > 0){
     videoList.pushTag("WINDOWA", n-1);
     int num = videoList.getNumTags("VIDEO");
     cout << num << " videos in WINDOWA" << endl;
     if(num > 0){
     windowA.resize(num);
     for(int i = 0; i < num; i++){
     cout << videoList.getValue("VIDEO", "", i) << endl;
     windowA[i].load(videoList.getValue("VIDEO", "", i));
     windowA[i].play();
     }
     }
     videoList.popTag();
     }
     n = videoList.getNumTags("WINDOWB:VIDEO");
     if(n > 0){
     videoList.pushTag("WINDOWB", n-1);
     int num = videoList.getNumTags("VIDEO");
     cout << num << " videos in WINDOWB" << endl;
     if(num > 0){
     windowB.resize(num);
     for(int i = 0; i < num; i++){
     cout << videoList.getValue("VIDEO", "", i) << endl;
     windowB[i].load(videoList.getValue("VIDEO", "", i));
     windowB[i].play();
     }
     }
     videoList.popTag();
     }
     */

}

//--------------------------------------------------------------
void vhpGame::update(){
    (*this.*currentUpdate)();
}

//--------------------------------------------------------------
void vhpGame::draw(){
    (*this.*currentDraw)();
}

//--------------------------------------------------------------
void vhpGame::set(int &_state){
    switch (_state) {
            
        case SCREENSAVER:
            cout << "SCREENSAVER" << endl;
            if (state == PLAYERMENU) {
                backToScreensaver();
            } else {
                setScreensaver();
            }
            break;
            
        case PLAYERMENU:
            setPlayerMenu();
            break;
            
        case STANDBY:
            state = STANDBY;
            currentUpdate = &vhpGame::updateStandby;
            currentDraw = &vhpGame::drawStandby;
            break;
            
        case FIRSTLEVEL:
            state = FIRSTLEVEL;
            currentUpdate = &vhpGame::updateFirstLevel;
            currentDraw = &vhpGame::drawFirstLevel;
            break;
            
        case SECONDLEVEL:
            state = SECONDLEVEL;
            currentUpdate = &vhpGame::updateSecondLevel;
            currentDraw = &vhpGame::drawSecondLevel;
            break;
            
        case THIRDLEVEL:
            state = THIRDLEVEL;
            currentUpdate = &vhpGame::updateThirdLevel;
            currentDraw = &vhpGame::drawThirdLevel;
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void vhpGame::toggleScale(){
    if (scale==1.0) {
        scale = 3.0;
        xogadores.scale = 3.0;
    } else {
        scale = 1.0;
        xogadores.scale = 1.0;
    }
}

// ScreenSaver -------------------------------------------------
void vhpGame::setScreensaver(){
    cout << "setScreensaver()" << endl;
    state = SCREENSAVER;
    logos.start();
    logos.play();
    ofAddListener(vhpScreenSaver::onClick, this, &vhpGame::set);
    currentUpdate = &vhpGame::updateScreenSaver;
    currentDraw = &vhpGame::drawScreenSaver;
}

// -------------------------------------------------------------
void vhpGame::backToScreensaver(){
    cout << "backToScreensaver()" << endl;
    state = SCREENSAVER;
    logos.start();
    logos.play();
    currentUpdate = &vhpGame::updatePlayerMenuFadeIn;
    currentDraw = &vhpGame::drawPlayerMenuFadeOut;
}

//--------------------------------------------------------------
void vhpGame::updateScreenSaver(){
    logos.update();
    /*
    for (int i = 0; i<background.size(); i++) {
        background[i].update();
    }*/
    /*
     for (int i = 0; i<windowA.size(); i++) {
     windowA[i].update();
     }
     for (int i = 0; i<windowB.size(); i++) {
     windowB[i].update();
     }
     */
}

//--------------------------------------------------------------
void vhpGame::drawScreenSaver(){
   
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    logos.draw(0, 0);
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofPopStyle();
    
    ofPushStyle();
    ofSetColor(255, 255, 255);
    TTF.drawString("Framerate: "+ ofToString(ofGetFrameRate()), 20, 20);
    ofPopStyle();
    
    /*for (int i = 0; i<background.size(); i++) {
        background[i].draw(width*i, 0);
    }
    start.display();
    */
    /*
     for (int i = 0; i<windowA.size(); i++) {
     windowA[i].draw(width + 360*i, 100, 360, 289);
     }
     for (int i = 0; i<windowB.size(); i++) {
     windowB[i].draw(width + 360*i, 489, 360, 289);
     }
     */
    
}

//--------------------------------------------------------------
void vhpGame::stopScreenSaver(){
    ofRemoveListener(vhpScreenSaver::onClick, this, &vhpGame::set);
}

// PlayerMenu --------------------------------------------------
void vhpGame::setPlayerMenu(){
    cout << "setPlayerMenu()" << endl;
    stopScreenSaver();
    state = PLAYERMENU;
    xogadores.init();
    
    ofAddListener(vhpThread::timeOut, this, &vhpGame::set);
    
    currentUpdate = &vhpGame::updatePlayerMenuFadeIn;
    currentDraw = &vhpGame::drawPlayerMenuFadeIn;
}

//--------------------------------------------------------------
void vhpGame::updatePlayerMenuFadeIn(){
    logos.update();
    xogadores.update();
}

//--------------------------------------------------------------
void vhpGame::updatePlayerMenu(){
    xogadores.update();
}

//--------------------------------------------------------------
void vhpGame::drawPlayerMenuFadeIn(){
    bufferTex.loadData(logos.video.getPixels(), width, height, GL_RGB);
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    shaderMixture.begin();
    shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    shaderMixture.setUniform1f("mixture", alpha/255.0);
    xogadores.draw(0, 0);
    shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofSetColor(0, 0, 0);
    TTF.drawString("Framerate: "+ ofToString(ofGetFrameRate()), 20, 20);
    ofPopStyle();
    alpha += alpha_increment;
    if (alpha>=255) {
        cout << "alpha: 255" << endl;
        alpha = 255;
        currentUpdate = &vhpGame::updatePlayerMenu;
        currentDraw = &vhpGame::drawPlayerMenu;
        logos.stop();
    }
}

//--------------------------------------------------------------
void vhpGame::drawPlayerMenuFadeOut(){
    bufferTex.loadData(logos.video.getPixels(), width, height, GL_RGB);
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    shaderMixture.begin();
    shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    shaderMixture.setUniform1f("mixture", alpha/255.0);
    xogadores.draw(0, 0);
    shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofSetColor(0, 0, 0);
    TTF.drawString("Framerate: "+ ofToString(ofGetFrameRate()), 20, 20);
    ofPopStyle();
    alpha -= alpha_increment;
    if (alpha<=0) {
        cout << "alpha: 0" << endl;
        alpha = 0;
        currentUpdate = &vhpGame::updateScreenSaver;
        currentDraw = &vhpGame::drawScreenSaver;
        ofAddListener(vhpScreenSaver::onClick, this, &vhpGame::set);
        xogadores.stop();
    }
}


//--------------------------------------------------------------
void vhpGame::drawPlayerMenu(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    xogadores.draw(0, 0);
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofSetColor(0, 0, 0);
    TTF.drawString("Framerate: "+ ofToString(ofGetFrameRate()), 20, 20);
    ofPopStyle();
}


// Standby ------------------------------------------------------
void vhpGame::updateStandby(){
    
}

//--------------------------------------------------------------
void vhpGame::drawStandby(){
    
}


// ScreenSaver -------------------------------------------------
void vhpGame::updateFirstLevel(){
    
}

//--------------------------------------------------------------
void vhpGame::drawFirstLevel(){
    
}


// SecondLevel -------------------------------------------------
void vhpGame::updateSecondLevel(){
    
}

//--------------------------------------------------------------
void vhpGame::drawSecondLevel(){
    
}


// ThirdLevel --------------------------------------------------
void vhpGame::updateThirdLevel(){
    
}

//--------------------------------------------------------------
void vhpGame::drawThirdLevel(){
    
}

/* EVENTS */

//--------------------------------------------------------------
void vhpGame::keyReleased(int _key){
    if ((_key=='s')||(_key=='S')) toggleScale();
}

//--------------------------------------------------------------
void vhpGame::mousePressed(int _x, int _y, int _button){

}

//--------------------------------------------------------------
void vhpGame::mouseReleased(int _x, int _y, int _button){

}
