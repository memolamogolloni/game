#include "vhpGame.h"

// Constructor --------------------------------------------------

vhpGame::vhpGame(){

}

// Destructor ---------------------------------------------------

vhpGame::~vhpGame(){
    if (state==SCREENSAVER) ofRemoveListener(vhpScreenSaver::onClick, this, &vhpGame::set);
    /*{
        ofRemoveListener(vhpButton::onActive, this, &vhpGame::set);
        myTest.stop();
    }*/
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
    
    // bufferTex.allocate(width, height, GL_RGB);
	
	/*
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
    */
    alpha = 0;
    alpha_increment = 15;
    
    TTF.loadFont("fonts/mono.ttf", 7);
    
    cout << "loading videoList.xml" << endl;
    if( videoList.loadFile("videoList.xml") ){
        cout << "videoList.xml loaded!" << endl;
    }else{
        cout << "unable to load videoList.xml check data/ folder" << endl;
    }
    
    logos.setup(videoList, "LOGOS", SCREENSAVER, PLAYERMENU);
    xogadores.setup(&controlXogadores, videoList, "XOGADORES",PLAYERMENU, STANDBY);
    espera.setup(&controlEspera, videoList, "ESPERA", STANDBY, LEVELMENU);
    niveis.setup(&controlNiveis, videoList, "NIVEIS", LEVELMENU, GAME);
    xogo.setup(&controlXogo, videoList, "XOGO", GAME, RANKING);
    
    logosLoaded = false;
    xogadoresLoaded = false;
    esperaLoaded = false;
    niveisLoaded = false;
    xogoLoaded = false;
    
    mensajeria.setOSC(videoList, "SENDER1");
    xogo.mensajeria.setOSC(videoList, "SENDER2");
    
    currentUpdate = &vhpGame::loadScreenSaver;
    currentDraw = &vhpGame::drawLoading;
    
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
    cout << "set " << _state << endl;
    mensajeria.send("set", _state);
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
                // Neste punto hay duas opcións que o xogador escollera 4
                // polo que habería que esperar á resposta do outro equipo,
                // ou que escollera 1 ou 2 polo que habería que ir directamente
                // á seleccion de nivel
                if (xogadores.selected==4) {
                    fadeInStandby();
                } else {
                    fadeInDirectLevelMenu();
                }
            } else {
                goToStandby();
            }
            break;
            
        case LEVELMENU:
            
            cout << "LEVELMENU" << endl;
            
            if (state == STANDBY) {
                fadeInLevelMenu();
            } else {
                goToLevelMenu();
            }
            break;
            
        case GAME:
            
            cout << "GAME" << endl;
            
            if (state == LEVELMENU) {
                fadeInGame();
            } else {
                if (state == SCREENSAVER) {
                    stopScreenSaver();
                }
                goToGame();
            }
            break;

        /*
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
        */
    }
}

//--------------------------------------------------------------

void vhpGame::toggleScale(){
    if (scale==1.0) {
        scale = 3.0;
    } else {
        scale = 1.0;
    }
}

// Loading -----------------------------------------------------

void vhpGame::loadScreenSaver(){
    if (!logosLoaded) {
        if (logos.video.isLoaded()) {
            logosLoaded = true;
            cout << "logos video is loaded" << endl;
        }
    } else {
        state = SCREENSAVER;
        set(state);
    }
}

void vhpGame::loadGame(){
    if (!xogadoresLoaded) {
        if (xogadores.video.isLoaded()) {
            xogadoresLoaded = true;
            cout << "xogadores video is loaded" << endl;
        }
    } else if (!esperaLoaded) {
        if (espera.video.isLoaded()) {
            esperaLoaded = true;
            cout << "espera video is loaded" << endl;
        }
    } else if (!niveisLoaded) {
        if (niveis.video.isLoaded()) {
            niveisLoaded = true;
            cout << "niveis video is loaded" << endl;
        }
    }  else if (!xogoLoaded) {
        if(!xogo.loaded) {
            xogo.load();
        } else {
            xogoLoaded = true;
            ofAddListener(vhpScreenSaver::onClick, this, &vhpGame::set);
            cout << "xogo is loaded" << endl;
        }
    }
}
//--------------------------------------------------------------

void vhpGame::drawLoading(){
    ofBackground(0, 0, 0);
    drawFrameRate();
}

// ScreenSaver -------------------------------------------------

void vhpGame::initScreenSaver(){
    cout << "SCREENSAVER" << endl;
    state = SCREENSAVER;
    logos.start();
    if (xogoLoaded) ofAddListener(vhpScreenSaver::onClick, this, &vhpGame::set);
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
    loadGame();
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
    if (!xogoLoaded) {
        ofPushStyle();
        ofSetColor(255, 255, 255);
        TTF.drawString("Loading", 20, 40);
        ofPopStyle();
    }
    
}

// PlayerMenu --------------------------------------------------

void vhpGame::initPlayerMenu(){
    cout << "PLAYERMENU" << endl;
    state = PLAYERMENU;
    xogadores.start();
    ofAddListener(vhpPmThread::timeOut, this, &vhpGame::set);
    ofAddListener(vhpPlayerMenu::playersNumber, this, &vhpGame::set);
}

void vhpGame::stopPlayerMenu(){
    xogadores.stop();
    ofRemoveListener(vhpPmThread::timeOut, this, &vhpGame::set);
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
    
    //bufferTex.loadData(logos.video.getPixels(), width, height, GL_RGB);
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    logos.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    xogadores.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
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
   // bufferTex.loadData(logos.video.getPixels(), width, height, GL_RGB);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    xogadores.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    logos.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
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
    cout << "ofAddListener vhpOSC::playersReceived" << endl;
    ofAddListener(vhpOSC::playersReceived, this, &vhpGame::set);
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
    mensajeria.send(xogadores.selected);
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
    mensajeria.update();
}

void vhpGame::drawStandbyIn(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", xogadores.fbo.getTexture(), 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    xogadores.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    espera.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
    ofPopStyle();
    
    drawFrameRate();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        cout << "alpha: 255" << endl;
        alpha = 255;
        initStandby();
        currentUpdate = &vhpGame::updateStandby;
        currentDraw = &vhpGame::drawStandby;
        xogadores.stop();
        xogadores.pause();
    }
    
    mensajeria.update();
}

void vhpGame::drawStandbyOut(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", xogadores.fbo.getTexture(), 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    espera.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    xogadores.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
    ofPopStyle();
    
    drawFrameRate();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        cout << "alpha: 0" << endl;
        alpha = 0;
        xogadores.start();
        ofAddListener(vhpPmThread::timeOut, this, &vhpGame::set);
        currentUpdate = &vhpGame::updatePlayerMenu;
        currentDraw = &vhpGame::drawPlayerMenu;
    }
}

// PlayerMenu --------------------------------------------------

void vhpGame::initLevelMenu(){
    cout << "LEVELMENU" << endl;
    state = LEVELMENU;
    niveis.start();
    ofAddListener(vhpLmThread::timeOut, this, &vhpGame::set);
    ofAddListener(vhpLevelMenu::levelSelection, this, &vhpGame::set);
}

void vhpGame::stopLevelMenu(){
    xogadores.stop();
    ofRemoveListener(vhpLmThread::timeOut, this, &vhpGame::set);
    ofRemoveListener(vhpLevelMenu::levelSelection, this, &vhpGame::set);
}

void vhpGame::goToLevelMenu(){
    cout << "goToLevelMenu()" << endl;
    initLevelMenu();
    niveis.play();
    currentUpdate = &vhpGame::updateLevelMenu;
    currentDraw = &vhpGame::drawLevelMenu;
}

void vhpGame::fadeInLevelMenu(){
    cout << "fadeInLevelMenu()" << endl;
    stopStandby();
    niveis.play();
    currentUpdate = &vhpGame::updateLevelMenuInOut;
    currentDraw = &vhpGame::drawLevelMenuIn;
}

void vhpGame::fadeInDirectLevelMenu(){
    cout << "fadeInLevelMenu()" << endl;
    stopPlayerMenu();
    niveis.play();
    currentUpdate = &vhpGame::updateDirectLevelMenuInOut;
    currentDraw = &vhpGame::drawLevelMenuIn;
}

void vhpGame::fadeOutLevelMenu(){
    cout << "fadeOutLevelMenu()" << endl;
    stopLevelMenu();
    espera.play();
    currentUpdate = &vhpGame::updateLevelMenuInOut;
    currentDraw = &vhpGame::drawLevelMenuOut;
}

//--------------------------------------------------------------

void vhpGame::updateLevelMenu(){
    niveis.update();
}

void vhpGame::updateLevelMenuInOut(){
    espera.update();
    niveis.update();
}

void vhpGame::updateDirectLevelMenuInOut(){
    xogadores.update();
    niveis.update();
}

//--------------------------------------------------------------

void vhpGame::drawLevelMenu(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    niveis.draw(0, 0);
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofPopStyle();
    
    drawFrameRate();
    
}

void vhpGame::drawLevelMenuIn(){
    //bufferTex.loadData(espera.video.getPixels(), width, height, GL_RGB);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    espera.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    niveis.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
    ofPopStyle();
    
    drawFrameRate();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        cout << "alpha: 255" << endl;
        alpha = 255;
        initLevelMenu();
        currentUpdate = &vhpGame::updateLevelMenu;
        currentDraw = &vhpGame::drawLevelMenu;
        espera.pause();
    }
}

void vhpGame::drawLevelMenuOut(){
    //bufferTex.loadData(espera.video.getPixels(), width, height, GL_RGB);
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", bufferTex, 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    niveis.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    espera.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
    ofPopStyle();
    
    drawFrameRate();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        cout << "alpha: 0" << endl;
        alpha = 0;
        initStandby();
        currentUpdate = &vhpGame::updateStandby;
        currentDraw = &vhpGame::drawStandby;
        niveis.pause();
    }
}

// Game -------------------------------------------------------

void vhpGame::initGame(){
    cout << "GAME" << endl;
    state = GAME;
    xogo.start();
    // PENDING! ofAddListener(vhpThread::timeOut, this, &vhpGame::set);
    // cout << "ofAddListener vhpOSC::playersReceived" << endl;
    // ofAddListener(vhpOSC::playersReceived, this, &vhpGame::set);
}

void vhpGame::stopGame(){
    xogo.stop();
    // PENDING! ofRemoveListener(vhpThread::timeOut, this, &vhpGame::set);
}

void vhpGame::goToGame(){
    cout << "goToGame()" << endl;
    initGame();
    xogo.play();
    currentUpdate = &vhpGame::updateGame;
    currentDraw = &vhpGame::drawGame;
}

void vhpGame::fadeInGame(){
    cout << "fadeInGame()" << endl;
    stopLevelMenu();
    niveis.pause();
    xogo.play();
    alpha = 0;
    //mensajeria.send(xogadores.selected);
    currentUpdate = &vhpGame::updateGameInOut;
    currentDraw = &vhpGame::drawGameIn;
}

void vhpGame::fadeOutGame(){
    cout << "fadeOutStandby()" << endl;
    stopGame();
    xogo.play();
    currentUpdate = &vhpGame::updateGameInOut;
    currentDraw = &vhpGame::drawGameOut;
}

//--------------------------------------------------------------

void vhpGame::updateGame(){
    xogo.update();
}

void vhpGame::updateGameInOut(){
    niveis.update();
    xogo.update();
}

//--------------------------------------------------------------

void vhpGame::drawGame(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    xogo.draw(0, 0);
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofPopStyle();
    
    drawFrameRate();
    //mensajeria.update();
}

void vhpGame::drawGameIn(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", niveis.fbo.getTexture(), 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    niveis.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    xogo.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
    ofPopStyle();
    
    drawFrameRate();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        cout << "alpha: 255" << endl;
        alpha = 255;
        initGame();
        currentUpdate = &vhpGame::updateGame;
        currentDraw = &vhpGame::drawGame;
        niveis.stop();
        niveis.pause();
    }
    
    //mensajeria.update();
}

void vhpGame::drawGameOut(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    //shaderMixture.begin();
    //shaderMixture.setUniformTexture("tex1", niveis.fbo.getTexture(), 1);
    //shaderMixture.setUniform1f("mixture", alpha/255.0);
    xogo.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    niveis.draw(0, 0);
    ofSetColor(255, 255, 255);
    //shaderMixture.end();
    fullScreen.end();
    fullScreen.draw(0, 0, width * scale, height * scale/3);
    ofDisableAlphaBlending();
    ofPopStyle();
    
    drawFrameRate();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        cout << "alpha: 0" << endl;
        alpha = 0;
        niveis.start();
        //ofAddListener(vhpPmThread::timeOut, this, &vhpGame::set);
        currentUpdate = &vhpGame::updateLevelMenu;
        currentDraw = &vhpGame::drawLevelMenu;
    }
}

//--------------------------------------------------------------

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
    } else if (_key=='3') {
        int scene = STANDBY;
        set(scene);
    } else if (_key=='4') {
        int scene = LEVELMENU;
        set(scene);
    } else if (_key=='5') {
        int scene = GAME;
        set(scene);
    }

}

//--------------------------------------------------------------

void vhpGame::mousePressed(int _x, int _y, int _button){
    float y = _y*3/scale;
    float x = _x*3/scale;
    switch (state) {
            
        case SCREENSAVER:
            logos.touchPressed();
            break;
            
        case PLAYERMENU:
            xogadores.touchPressed(x, y);
            break;
            
        case STANDBY:
            espera.touchPressed();
            break;
            
        case LEVELMENU:
            niveis.touchPressed(x, y);
            break;
            
        case GAME:
            xogo.touchPressed(x, y);
            break;
    }

}

//--------------------------------------------------------------

void vhpGame::mouseReleased(int _x, int _y, int _button){

}
