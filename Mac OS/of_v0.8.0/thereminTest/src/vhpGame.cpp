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

    #ifdef TARGET_OPENGLES
        cout << "TARGET_OPENGLES" << endl;
        shaderMixture.load("shadersGL2/mixture.vert","shadersGL2/mixture.frag");
    #else
    if(ofGetGLProgrammableRenderer()){
        cout << "GL3" << endl;
        shaderMixture.load("shadersGL2/mixture.vert", "shadersGL2/mixture.frag");
    }else{
        cout << "GL2" << endl;
        shaderMixture.load("shadersGL2/mixture.vert", "shadersGL2/mixture.frag");
    }
    #endif
    alpha = 0;
    alpha_increment = 15;
    
    TTF.loadFont("mono.ttf", 7);
    
    cout << "loading videoList.xml" << endl;
    if( videoList.loadFile("videoList.xml") ){
        cout << "videoList.xml loaded!" << endl;
    }else{
        cout << "unable to load videoList.xml check data/ folder" << endl;
    }
    
    logos.setup(videoList, "LOGOS", SCREENSAVER, PLAYERMENU);
    xogadores.setup(&controlXogadores, videoList, "XOGADORES",PLAYERMENU, STANDBY);
    espera.setup(videoList, "ESPERA", PLAYERMENU, STANDBY);
    
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

void vhpGame::drawFrameRate(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    TTF.drawString("Framerate: "+ ofToString(ofGetFrameRate()), 20, 20);
    ofPopStyle();
}

//--------------------------------------------------------------

void vhpGame::set(int &_state){
    switch (_state) {
            
        case SCREENSAVER:
            if (state == PLAYERMENU) {
                fadeOutPlayerMenu();
            } else {
                goToScreenSaver();
            }
            break;
        case PLAYERMENU:
            if (state == SCREENSAVER) {
                fadeInPlayerMenu();
            } else {
                goToPlayerMenu();
            }
            break;
        case STANDBY:
            if (state == PLAYERMENU) {
                cout << "ole" << endl;
                fadeInStandby();
            } else {
                goToStandby();
            }
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

void vhpGame::initScreenSaver(){
    cout << "SCREENSAVER" << endl;
    state = SCREENSAVER;
    logos.start();
    ofAddListener(vhpScreenSaver::onClick, this, &vhpGame::set);
}

void vhpGame::stopScreenSaver(){
    logos.stop();
    ofRemoveListener(vhpScreenSaver::onClick, this, &vhpGame::set);
}

void vhpGame::goToScreenSaver(){
    cout << "goToScreenSaver()" << endl;
    initScreenSaver();
    logos.play();
    currentUpdate = &vhpGame::updateScreenSaver;
    currentDraw = &vhpGame::drawScreenSaver;
}

//--------------------------------------------------------------

void vhpGame::updateScreenSaver(){
    logos.update();
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
    
    drawFrameRate();
    
}

// PlayerMenu --------------------------------------------------

void vhpGame::initPlayerMenu(){
    cout << "PLAYERMENU" << endl;
    state = PLAYERMENU;
    xogadores.start();
    ofAddListener(vhpThread::timeOut, this, &vhpGame::set);
    ofAddListener(vhpPlayerMenu::playersNumber, this, &vhpGame::set);
}

void vhpGame::stopPlayerMenu(){
    xogadores.stop();
    ofRemoveListener(vhpThread::timeOut, this, &vhpGame::set);
    ofRemoveListener(vhpPlayerMenu::playersNumber, this, &vhpGame::set);
}

void vhpGame::goToPlayerMenu(){
    cout << "goToPlayerMenu()" << endl;
    initPlayerMenu();
    xogadores.play();
    currentUpdate = &vhpGame::updatePlayerMenu;
    currentDraw = &vhpGame::drawPlayerMenu;
}

void vhpGame::fadeInPlayerMenu(){
    cout << "fadeInPlayerMenu()" << endl;
    stopScreenSaver();
    xogadores.play();
    currentUpdate = &vhpGame::updatePlayerMenuInOut;
    currentDraw = &vhpGame::drawPlayerMenuIn;
}

void vhpGame::fadeOutPlayerMenu(){
    cout << "fadeOutPlayerMenu()" << endl;
    stopPlayerMenu();
    logos.play();
    currentUpdate = &vhpGame::updatePlayerMenuInOut;
    currentDraw = &vhpGame::drawPlayerMenuOut;
}

//--------------------------------------------------------------

void vhpGame::updatePlayerMenu(){
    xogadores.update();
}

void vhpGame::updatePlayerMenuInOut(){
    logos.update();
    xogadores.update();
}

//--------------------------------------------------------------

void vhpGame::drawPlayerMenu(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    xogadores.draw(0, 0);
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofPopStyle();
    
    drawFrameRate();
    
}

void vhpGame::drawPlayerMenuIn(){
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
    ofPopStyle();
    
    drawFrameRate();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        cout << "alpha: 255" << endl;
        alpha = 255;
        initPlayerMenu();
        currentUpdate = &vhpGame::updatePlayerMenu;
        currentDraw = &vhpGame::drawPlayerMenu;
        logos.pause();
    }
}

void vhpGame::drawPlayerMenuOut(){
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
    ofPopStyle();
    
    drawFrameRate();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        cout << "alpha: 0" << endl;
        alpha = 0;
        initScreenSaver();
        currentUpdate = &vhpGame::updateScreenSaver;
        currentDraw = &vhpGame::drawScreenSaver;
        xogadores.pause();
    }
}

// Standby ------------------------------------------------------

void vhpGame::initStandby(){
    cout << "STANDBY" << endl;
    state = STANDBY;
    espera.start();
    // PENDING! ofAddListener(vhpThread::timeOut, this, &vhpGame::set);
}

void vhpGame::stopStandby(){
    espera.stop();
    // PENDING! ofRemoveListener(vhpThread::timeOut, this, &vhpGame::set);
}

void vhpGame::goToStandby(){
    cout << "goToStandby()" << endl;
    initStandby();
    espera.play();
    currentUpdate = &vhpGame::updateStandby;
    currentDraw = &vhpGame::drawStandby;
}

void vhpGame::fadeInStandby(){
    cout << "fadeInStandby()" << endl;
    stopPlayerMenu();
    xogadores.pause();
    espera.play();
    alpha = 0;
    currentUpdate = &vhpGame::updateStandbyInOut;
    currentDraw = &vhpGame::drawStandbyIn;
}

void vhpGame::fadeOutStandby(){
    cout << "fadeOutStandby()" << endl;
    stopStandby();
    xogadores.play();
    currentUpdate = &vhpGame::updateStandbyInOut;
    currentDraw = &vhpGame::drawStandbyOut;
}

//--------------------------------------------------------------

void vhpGame::updateStandby(){
    espera.update();
}

void vhpGame::updateStandbyInOut(){
    xogadores.update();
    espera.update();
}

//--------------------------------------------------------------

void vhpGame::drawStandby(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    espera.draw(0, 0);
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofPopStyle();
    
    drawFrameRate();
    
}

void vhpGame::drawStandbyIn(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    shaderMixture.begin();
    shaderMixture.setUniformTexture("tex1", xogadores.fbo.getTextureReference(), 1);
    shaderMixture.setUniform1f("mixture", alpha/255.0);
    espera.draw(0, 0);
    shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofPopStyle();
    
    drawFrameRate();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        cout << "alpha: 255" << endl;
        alpha = 255;
        espera.start();
        currentUpdate = &vhpGame::updateStandby;
        currentDraw = &vhpGame::drawStandby;
        xogadores.stop();
        xogadores.pause();
    }
}

void vhpGame::drawStandbyOut(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    shaderMixture.begin();
    shaderMixture.setUniformTexture("tex1", xogadores.fbo.getTextureReference(), 1);
    shaderMixture.setUniform1f("mixture", alpha/255.0);
    espera.draw(0, 0);
    shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofPopStyle();
    
    drawFrameRate();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        cout << "alpha: 0" << endl;
        alpha = 0;
        xogadores.start();
        ofAddListener(vhpThread::timeOut, this, &vhpGame::set);
        currentUpdate = &vhpGame::updatePlayerMenu;
        currentDraw = &vhpGame::drawPlayerMenu;
    }
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
    if ((_key=='s')||(_key=='S')) {
        toggleScale();
    } else if (_key=='1') {
        int scene = SCREENSAVER;
        set(scene);
    } else if (_key=='2') {
        int scene = PLAYERMENU;
        set(scene);
    }
}

//--------------------------------------------------------------

void vhpGame::mousePressed(int _x, int _y, int _button){

}

//--------------------------------------------------------------

void vhpGame::mouseReleased(int _x, int _y, int _button){

}
