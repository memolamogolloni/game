#include "vhpGame.h"

// Constructor --------------------------------------------------
vhpGame::vhpGame(){

}

// Destructor ---------------------------------------------------
vhpGame::~vhpGame(){
    if (state==SCREENSAVER) ofRemoveListener(vhpScreenSaver::onClick, this, &vhpGame::set);
}

//--------------------------------------------------------------
void vhpGame::setup(){
    ofEnableAntiAliasing();
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    width = 1920;
    height = 1080;
    scale = 1.0;
    fullScreen.allocate(width*3, height, GL_RGBA);
    fullScreen.begin();
    ofClear(255,255,255, 0);
    fullScreen.end();
    
    alpha = 0.0;
    alpha_increment = 25.5;
    
    TTF.loadFont("fonts/mono.ttf", 7, true, true);
    
    cout << "loading settings.xml" << endl;
    if( settings.loadFile("settings.xml") ){
        cout << "settings.xml loaded!" << endl;
    }else{
        cout << "unable to load settings.xml check data/ folder" << endl;
    }
    
    mensajeria.setOSC(settings, "SENDER");
    
    logos.setup(SCREENSAVER, PLAYERMENU);
    resources.setup();
    xogadores.setup(PLAYERMENU, LEVELMENU, STANDBY);
    espera.setup(STANDBY, LEVELMENU);
    niveis.setup(LEVELMENU, GAME);
    xogo.setup(&mensajeria, GAME, RANKING);
    
    logosLoaded = false;
    resourcesLoaded = false;
    xogadoresLoaded = false;
    esperaLoaded = false;
    niveisLoaded = false;
    xogoLoaded = false;
    
    currentUpdate = &vhpGame::loadScreenSaver;
    currentDraw = &vhpGame::drawLoading;
    
    toggleScale();
    
    /*  OSC  */
    /* ----- */
    mensajeria.send("composition/disconnectall", 1);
    mensajeria.send("composition/deck1/select", 1);
    cout << "sending composition/disconnectall" << 1 << endl;
    cout << "sending composition/deck1/select" << 1 << endl;
    /* ----- */
    
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
void vhpGame::drawFrameRate(){
    /*
    ofPushStyle();
    ofSetColor(255, 255, 255);
    TTF.drawString("Framerate: "+ ofToString(ofGetFrameRate()), 20, 20);
    ofPopStyle();
    */
}

// Event Mangers -----------------------------------------------
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
                /*
                // Neste punto hay duas opcións que o xogador escollera 4
                // polo que habería que esperar á resposta do outro equipo,
                // ou que escollera 1 ou 2 polo que habería que ir directamente
                // á seleccion de nivel
                if (xogadores.selected==4) {
                    fadeInStandby();
                } else {
                    fadeInDirectLevelMenu();
                }
                 
                 */
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
void vhpGame::screenSaverOnClick(int &_s){
    cout << "screensaver clicked! state: " << _s << endl;
    
    /*  OSC  */
    /* ----- */
    mensajeria.send("layer2/clip1/connect", 1);
    cout << "sending layer2/clip1/connect" << 1 << endl;
    /* ----- */
    
    
    fadeInPlayerMenu();
}
void vhpGame::playerMenuOnSelect(int &_s){
    (xogadores.selected==1) ? xogo.IA = true : xogo.IA = false;
    cout << "players selected! state: " << _s << endl;
    if (_s == LEVELMENU) {
        cout << "LEVELMENU: " << LEVELMENU << endl;
        
        /*  OSC  */
        /* ----- */
        mensajeria.send("layer2/clip2/connect", 1);
        cout << "sending layer2/clip2/connect" << 1 << endl;
        /* ----- */
        
        fadeInDirectLevelMenu();
    } else if (_s == STANDBY){
        cout << "STANDBY: " << STANDBY << endl;
        fadeInStandby();
    }
}
void vhpGame::playerLevelOnSelect(int &_s){
    cout << "level selected! state: " << _s << endl;
    
    /*  OSC  */
    /* ----- */
    mensajeria.send("layer2/clip3/connect", 1);
    cout << "sending layer2/clip3/connect" << 1 << endl;
    /* ----- */
    xogo.level = _s;
    fadeInGame();
}
void vhpGame::standByOnCancel(int &_s){
    cout << "standBy clicked! state: " << _s << endl;
    fadeInPlayerMenu();
}
void vhpGame::onPlayersReceived(int &_s){
    cout << "Players from the other team received: " << _s << endl;
}
void vhpGame::gameOnRestart(int &_s){
    cout << "restart clicked! state: " << _s << endl;
    ofRemoveListener(vhpGameCore::onRestart, this, &vhpGame::gameOnRestart);
    goToScreenSaver();
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
        if (!logos.loaded) {
            logos.load();
        } else {
            logosLoaded = true;
            cout << "logos video is loaded" << endl;
        }
    } else {
        
        /*  OSC  */
        /* ----- */
        mensajeria.send("layer1/clip1/connect", 1);
        cout << "sending layer1/clip1/connect" << 1 << endl;
        /* ----- */
        
        state = SCREENSAVER;
        set(state);
    }
}
void vhpGame::loadGame(){
    if (!resourcesLoaded) {
        if(!resources.loaded) {
            resources.load();
        } else {
            xogadores.setupResources(&resources.images[2], &resources.images[3], &resources.images[4], &resources.petamuti);
            xogadores.setupFonts(&resources.TTF, &resources.TTFB, &resources.TTFM);
            xogadores.setupSounds(&resources.selectSound, &resources.aceptSound);
            niveis.setupResources(&resources.images[2], &resources.petamuti);
            niveis.setupFonts(&resources.TTF, &resources.TTFB);
            xogo.setupResources(&resources.caritas[0], &resources.caritas[1], &resources.images[2], &resources.images[3], &resources.images[4], &resources.petamuti, &resources.flecha);
            xogo.setupFonts(&resources.TTF, &resources.TTFB, &resources.TTFM);
            resourcesLoaded = true;
            cout << "resources is loaded" << endl;
        }
    } else if (!xogadoresLoaded) {
        if(!xogadores.loaded) {
            xogadores.load();
        } else {
            xogadoresLoaded = true;
            cout << "xogadores is loaded" << endl;
        }
    } else if (!esperaLoaded) {
        if(!espera.loaded) {
            espera.load();
        } else {
            esperaLoaded = true;
            cout << "espera is loaded" << endl;
        }
    } else if (!niveisLoaded) {
        if(!niveis.loaded) {
            niveis.load();
        } else {
            niveisLoaded = true;
            cout << "niveis is loaded" << endl;
        }
    }  else if (!xogoLoaded) {
        if(!xogo.loaded) {
            xogo.load();
        } else {
            xogoLoaded = true;
            cout << "ofAddListener vhpScreenSaver::onClick" << endl;
            ofAddListener(vhpScreenSaver::onClick, this, &vhpGame::screenSaverOnClick);
            cout << "xogo is loaded" << endl;
            logos.finishLoading = true;
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
    if (xogoLoaded) {
        cout << "ofAddListener vhpScreenSaver::onClick" << endl;
        ofAddListener(vhpScreenSaver::onClick, this, &vhpGame::screenSaverOnClick);
    }
}
void vhpGame::stopScreenSaver(){
    logos.stop();
    cout << "ofRemoveListener onClick" << endl;
    ofRemoveListener(vhpScreenSaver::onClick, this, &vhpGame::screenSaverOnClick);
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
    /*
    if (!xogoLoaded) {
        ofPushStyle();
        ofSetColor(255, 255, 255);
        TTF.drawString("Loading", 20, 40);
        ofPopStyle();
    }
    */
}

// PlayerMenu --------------------------------------------------
void vhpGame::initPlayerMenu(){
    cout << "PLAYERMENU INIT" << endl;
    state = PLAYERMENU;
    //xogadores.start();
    //ofAddListener(vhpPmThread::timeOut, this, &vhpGame::set);
    cout << "ofAddListener playersNumber" << endl;
    ofAddListener(vhpPlayerMenu::playersNumber, this, &vhpGame::playerMenuOnSelect);
}
void vhpGame::stopPlayerMenu(){
    xogadores.stop();
    //ofRemoveListener(vhpPmThread::timeOut, this, &vhpGame::set);
    cout << "ofRemoveListener playersNumber" << endl;
    ofRemoveListener(vhpPlayerMenu::playersNumber, this, &vhpGame::playerMenuOnSelect);
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
    
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    logos.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    xogadores.draw(0, 0);
    ofSetColor(255, 255, 255);
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
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    xogadores.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    logos.draw(0, 0);
    ofSetColor(255, 255, 255);
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
    }
}

// Standby ------------------------------------------------------
void vhpGame::initStandby(){
    cout << "STANDBY" << endl;
    state = STANDBY;
    espera.start();
    cout << "ofAddListener vhpOSC::playersReceived" << endl;
    ofAddListener(vhpOSC::playersReceived, this, &vhpGame::onPlayersReceived);
    cout << "ofAddListener vhpStandby::onClick" << endl;
    ofAddListener(vhpStandby::onCancel, this, &vhpGame::standByOnCancel);
}
void vhpGame::stopStandby(){
    espera.stop();
    ofRemoveListener(vhpOSC::playersReceived, this, &vhpGame::onPlayersReceived);
    ofRemoveListener(vhpStandby::onCancel, this, &vhpGame::standByOnCancel);
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
    xogadores.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    espera.draw(0, 0);
    ofSetColor(255, 255, 255);
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
    }
    
    mensajeria.update();
}
void vhpGame::drawStandbyOut(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    espera.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    xogadores.draw(0, 0);
    ofSetColor(255, 255, 255);
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
        currentUpdate = &vhpGame::updatePlayerMenu;
        currentDraw = &vhpGame::drawPlayerMenu;
    }
}

// PlayerMenu --------------------------------------------------
void vhpGame::initLevelMenu(){
    cout << "LEVELMENU INIT" << endl;
    state = LEVELMENU;
    niveis.start();
    //ofAddListener(vhpLmThread::timeOut, this, &vhpGame::set);
    ofAddListener(vhpLevelMenu::levelSelection, this, &vhpGame::playerLevelOnSelect);
}
void vhpGame::stopLevelMenu(){
    xogadores.stop();
    //ofRemoveListener(vhpLmThread::timeOut, this, &vhpGame::set);
    ofRemoveListener(vhpLevelMenu::levelSelection, this, &vhpGame::playerLevelOnSelect);
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
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    espera.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    niveis.draw(0, 0);
    ofSetColor(255, 255, 255);
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
    }
}
void vhpGame::drawDirectLevelMenuIn(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    xogadores.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    niveis.draw(0, 0);
    ofSetColor(255, 255, 255);
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
    }
}
void vhpGame::drawLevelMenuOut(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    niveis.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    espera.draw(0, 0);
    ofSetColor(255, 255, 255);
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
    }
}

// Game -------------------------------------------------------
void vhpGame::initGame(){
    cout << "GAME" << endl;
    state = GAME;
    xogo.start();
    // PENDING! ofAddListener(vhpThread::timeOut, this, &vhpGame::set);
    cout << "ofAddListener vhpGameCore::onRestart" << endl;
    ofAddListener(vhpGameCore::onRestart, this, &vhpGame::gameOnRestart);
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
    //niveis.pause();
    xogo.play();
    alpha = 0;
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
    niveis.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    xogo.draw(0, 0);
    ofSetColor(255, 255, 255);
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
        
        /*  OSC  */
        /* ----- */
        mensajeria.send("composition/deck2/select", 1);
        mensajeria.send("layer1/clip"+ ofToString(xogo.level) +"/connect", 1);
        cout << "sending composition/deck2/select" << 1 << endl;
        cout << "layer1/clip"+ ofToString(xogo.level) +"/connect" << 1 << endl;
        /* ----- */
        
    }
}
void vhpGame::drawGameOut(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    fullScreen.begin();
    xogo.draw(0, 0);
    ofSetColor(255,255,255,alpha);
    niveis.draw(0, 0);
    ofSetColor(255, 255, 255);
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
    } else if ((_key=='f')||(_key=='F')) {
        ofToggleFullscreen();
    } else if (xogo.loaded) {
        if (_key=='1') {
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
}

//--------------------------------------------------------------
void vhpGame::setTouchPosition(ofPoint _p){
	if ((_p.x>=0)&&(_p.x<=ofGetWindowWidth())) touchP.x = _p.x;
	if ((_p.y>=0)&&(_p.y<=ofGetWindowHeight())) touchP.y = _p.y;
}

void vhpGame::touchPressed(bool _t){
    float y = touchP.y*3/scale;
    float x = touchP.x*3/scale;
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

void vhpGame::touchReleased(bool _t){
    float y = touchP.y*3/scale;
    float x = touchP.x*3/scale;
    switch (state) {
            
        case SCREENSAVER:
            //logos.touchReleased();
            break;
            
        case PLAYERMENU:
            xogadores.touchReleased(x, y);
            break;
            
        case STANDBY:
            //espera.touchReleased();
            break;
            
        case LEVELMENU:
            //niveis.touchReleased(x, y);
            break;
            
        case GAME:
            xogo.touchReleased(x, y);
            break;
    }
}
