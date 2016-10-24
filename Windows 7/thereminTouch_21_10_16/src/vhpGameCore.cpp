#include "vhpGameCore.h"

// Constructor -------------------------------------------------

vhpGameCore::vhpGameCore(): scale(1.0), loaded(false), loading(false){

    wHeight = 223;
    wY = 335;
    wWidth[0][0] = 172;
    wWidth[0][1] = 146;
    wWidth[0][2] = 138;
    wWidth[0][3] = 131;
    wWidth[0][4] = 125;
    wWidth[0][5] = 120;
    wWidth[0][6] = 128;
    wWidth[1][6] = 172;
    wWidth[1][5] = 146;
    wWidth[1][4] = 138;
    wWidth[1][3] = 131;
    wWidth[1][2] = 125;
    wWidth[1][1] = 120;
    wWidth[1][0] = 128;
    
    wX[0][0] = 0;
    for (int i = 1; i < nWINDOWS; i++) {
        wX[0][i] = wX[0][i-1] + wWidth[0][i-1];
    }
    wX[1][0] = wX[0][6] + wWidth[0][6];
    for (int i = 1; i < nWINDOWS; i++) {
        wX[1][i] = wX[1][i-1] + wWidth[1][i-1];
    }
    
}

vhpGameCore::~vhpGameCore(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------
void vhpGameCore::setup(vhpGcThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene){
    
    // Añadir los vídeos desde el documento xml de settings
    int n = _videoList.getNumTags(_videoTag +":VIDEO");
    if(n > 0){
        _videoList.pushTag(_videoTag, n-1);
        int num = _videoList.getNumTags("VIDEO");
        cout << num << " videos in " << _videoTag << " " << _videoList.getValue("VIDEO", "", 0) << endl;
        if(num > 0) {
            for (int i = 0; i<num; i++) {
                videoFile.push_back(_videoList.getValue("VIDEO", "", i));
                video.push_back(ofVideoPlayer());
            }
        }
        _videoList.popTag();
    }
    
    // Añadir las imágenes Sueltas
    loadingSilge.push_back(&building);
    filesSingle.push_back("fondo-edificio");
    loadingSilge.push_back(&score[0]);
    filesSingle.push_back("red-bar");
    loadingSilge.push_back(&score[1]);
    filesSingle.push_back("blue-bar");
    loadingSilge.push_back(&ready);
    filesSingle.push_back("ready");
    loadingSilge.push_back(&steady);
    filesSingle.push_back("steady");
    loadingSilge.push_back(&go);
    filesSingle.push_back("go");
    loadingSilge.push_back(&tie);
    filesSingle.push_back("empate");
    loadingSilge.push_back(&winnerBackground[0]);
    filesSingle.push_back("red-wins");
    loadingSilge.push_back(&winnerBackground[1]);
    filesSingle.push_back("blue-wins");
    loadingSilge.push_back(&winnerButton[0]);
    filesSingle.push_back("red-wins-button");
    loadingSilge.push_back(&winnerButton[1]);
    filesSingle.push_back("blue-wins-button");
    loadingSilge.push_back(&purple);
    filesSingle.push_back("purple");
    loadingSilge.push_back(&yellow);
    filesSingle.push_back("yellow");
    loadingSilge.push_back(&blue);
    filesSingle.push_back("blue");
    loadingSilge.push_back(&green);
    filesSingle.push_back("green");
    loadingSilge.push_back(&iconosR);
    filesSingle.push_back("iconos-rojo");
    loadingSilge.push_back(&iconosA);
    filesSingle.push_back("iconos-azul");
    loadingSilge.push_back(&marcos);
    filesSingle.push_back("marcos");
    loadingSilge.push_back(&avisos);
    filesSingle.push_back("avisos");
    loadingSilge.push_back(&click);
    filesSingle.push_back("click");
    
    // Añadir las fuentes
    TTF.loadFont("fonts/titilliumweblight.ttf", 70);
    
    currentLoad = &vhpGameCore::loadVideo;
    
    // Inicializar las variables
    currentScene = _currentScene;   // SCREENSAVER
    targetScene = _targetScene;     // PLAYERMENU
    alpha = 0;
    alpha_increment = 5;
    
    pWindow.setup();
    
}
void vhpGameCore::initGame(){
    points[0] = 0;
    points[1] = 0;
    currentRound = 0;
    for (int i=0; i<2; i++) {
        for (int u=0; u<7; u++) {
            windowState[i][u] = pendingW;
        }
    }
    
}
void vhpGameCore::initRound(){
    cout << "initRound()" << endl;
    holdSteady = ceil(ofRandom(3));
    targetsShot = randomWindow();
    mensajeria.send("targetsShot", targetsShot);
    alpha = 0;
    for (int i=0; i<4; i++) {
        alphaWindow[i] = 0;
    }
    alpha_increment = 5;
    clicked[0] = 0;
    clicked[1] = 0;
    ok[0] = false;
    ok[1] = false;
    hold[0] = false;
    hold[1] = false;
    next[0] = false;
    next[1] = false;
    winner = 0;
    delay = 90;
}
void vhpGameCore::initPattern(){
    holdSteady = ceil(ofRandom(3));
    targetsShot = randomWindow();
    alpha = 0;
    for (int i=0; i<4; i++) {
        alphaWindow[i] = 0;
    }
    for (int i = 0; i<4; i++) {
        registeredPattern[0][i] = 7;
        registeredPattern[1][i] = 7;
    }
    alpha_increment = 5;
    clicked[0] = 0;
    clicked[1] = 0;
    ok[0] = false;
    ok[1] = false;
    hold[0] = false;
    hold[1] = false;
    next[0] = false;
    next[1] = false;
    winner = 0;
    delay = 90;
    for (int i = 0; i<nWINDOWS; i++) {
        windowState[0][i]= whiteW;
        windowState[1][i]= whiteW;
    }
}


// Precarga de todos los elementos -----------------------------
void vhpGameCore::load(){
    (*this.*currentLoad)();
}
void vhpGameCore::loadVideo(){
    if (!loaded) {
        int actual = videoFile.size() - 1;
        cout << "actual: " <<actual << " video.size(): " << video.size() << endl;
        if (actual>=0) {
            if(video[actual].isLoaded()) {
                cout << "video: " << videoFile[actual] << " is loaded!" << endl;
                videoFile.pop_back();
                loading = false;
            } else {
                if (!loading) {
                    video[actual].loadMovie(videoFile[actual]);
                    cout << "load video: " << videoFile[actual] << endl;
                    loading = true;
                }
            }
        } else {
            cout << "loading single videos finished!" << endl;
            width = video[0].getWidth();
            height = video[0].getHeight();
            cout << " video width: " << width << " height: " << height << endl;
            fbo.allocate(width, height, GL_RGBA);
            // clean FBO
            fbo.begin();
            ofClear(255,255,255, 0);
            fbo.end();
            loading = false;
            currentLoad = &vhpGameCore::loadSingle;
        }
    }
}
void vhpGameCore::loadSingle(){
    if (!loaded) {
        cout << "loadSingle() " << filesSingle.size() << endl;
        int actual = filesSingle.size() -1;
        if (actual>=0) {
            if(loadingSilge[actual]->isAllocated()) {
                loadingSilge.pop_back();
                filesSingle.pop_back();
                loading = false;
            } else {
                if (!loading) {
                    loadingSilge[actual]->loadImage("images/"+ filesSingle[actual] +".png");
                    cout << "images/"+ filesSingle[actual] +".png" << endl;
                    loading = true;
                }
            }
            
        } else {
            cout << "loading single images finished!" << endl;
            loaded = true;
        }
    }
}

// Comenzar e interrumpir los hilos y listeners de la escena ---

void vhpGameCore::start(){
    initGame();
}

void vhpGameCore::stop(){
}

// Dibujado y actualización variables --------------------------

void vhpGameCore::update(){
    (*this.*currentUpdate)();
}

void vhpGameCore::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Dibujado de elementos ---------------------------------------

void vhpGameCore::drawScore(){
    ofPushStyle();
    int diff = points[0] - points[1];
    int widthA = 700 + (100*diff);
    int widthB = 700 - (100*diff);
    score[0].draw(260, 925, widthA, 81);
    score[1].draw(260 + widthA, 925, widthB, 81);
    ofSetColor(255,255,255);
    ofSetLineWidth(1);
    ofLine(260 + widthA, 925+16, 260 + widthA, 925+16+49);
    ofPopStyle();
}
void vhpGameCore::drawGame(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    video[0].draw(0, 0);
    building.draw(0, 0);
    drawWindows();
    drawScore();
    ofPopStyle();
}
void vhpGameCore::drawWindows(){
    ofPushStyle();
    for (int i = 0; i<nWINDOWS; i++) {
        switch (windowState[0][i]) {
            case pendingW:
                iconosR.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                marcos.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            case lostW:
                iconosA.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            case wonW:
                iconosR.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            case whiteW:
                avisos.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            default:
                break;
        }
        switch (windowState[1][i]) {
            case pendingW:
                iconosA.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                marcos.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            case lostW:
                iconosR.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            case wonW:
                iconosA.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            case whiteW:
                avisos.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            default:
                break;
        }
    }
    ofPopStyle();
}

// Reproducir o detener la escena modificando currentUpdate ----

void vhpGameCore::play(){
    initRound();
    currentUpdate = &vhpGameCore::playReady;
    currentTouchPressed = &vhpGameCore::touchPressedGame;
    mensajeria.send("gamestate", 0);
    video[0].play();
}

void vhpGameCore::pause(){
    currentUpdate = &vhpGameCore::pause;
    video[0].stop();
}

// Procesado y actualización -----------------------------------

void vhpGameCore::playScreenSaver(){
    // Reproduce el video y lo dibuja en el FBO
    video[0].update();
    fbo.begin();
    drawGame();
    fbo.end();
}

void vhpGameCore::pauseScreenSaver(){
    // Al no hacer nada mantiene el FBO con el último fotograma reproducido
}

void vhpGameCore::loopScreenSaver(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    video[0].setPosition(_pos);
}

// Ready -----------------------------------
void vhpGameCore::playReady(){
    // Reproduce el video y lo dibuja en el FBO
    video[0].update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    ready.draw(0,0);
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>255) {
        alpha = 255;
        alpha_increment = -1 * alpha_increment;
    } else if (alpha<0) {
        alpha = 0;
        alpha_increment = -1 * alpha_increment;
        currentUpdate = &vhpGameCore::playSteady;
        mensajeria.send("gamestate", 1);
    }
}

// Steady -----------------------------------
void vhpGameCore::playSteady(){
    // Reproduce el video y lo dibuja en el FBO
    video[0].update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    steady.draw(0,0);
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        alpha_increment = -1 * alpha_increment;
    } else if (alpha<=0) {
        alpha = 0;
        alpha_increment = -1 * alpha_increment;
        if (holdSteady<=0) {
            currentUpdate = &vhpGameCore::playGo;
            mensajeria.send("gamestate", 2);
        }
        holdSteady --;
    }
}

// Go -----------------------------------
void vhpGameCore::playGo(){
    // Reproduce el video y lo dibuja en el FBO
    video[0].update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    go.draw(0,0);
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        alpha_increment = -1 * alpha_increment;
    } else if (alpha<=0) {
        alpha = 0;
        alpha_increment = -1 * alpha_increment;
        currentUpdate = &vhpGameCore::showWindow;
        mensajeria.send("gamestate", 3);
    }
}

// Show Window -----------------------------------
void vhpGameCore::showWindow(){
    // pequeño delay antes de enseñar la ventana
    // Reproduce el video y lo dibuja en el FBO
    video[0].update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    avisos.drawSubsection(wX[0][targetsShot], wY, wWidth[0][targetsShot], wHeight, wX[0][targetsShot], wY, wWidth[0][targetsShot], wHeight);
    avisos.drawSubsection(wX[1][targetsShot], wY, wWidth[1][targetsShot], wHeight, wX[1][targetsShot], wY, wWidth[1][targetsShot], wHeight);
    if (clicked[0]!=0) {
        ofSetColor(255,255,255,alphaWindow[0]);
        click.drawSubsection(wX[0][clicked[0]], wY, wWidth[0][clicked[0]], wHeight, wX[0][clicked[0]], wY, wWidth[0][clicked[0]], wHeight);
        alphaWindow[0] += 35;
        if (alphaWindow[0]>=255) alphaWindow[0] = 255;
    }
    if (clicked[1]!=0) {
        ofSetColor(255,255,255,alphaWindow[1]);
        click.drawSubsection(wX[1][clicked[1]], wY, wWidth[1][clicked[1]], wHeight, wX[1][clicked[1]], wY, wWidth[1][clicked[1]], wHeight);
        alphaWindow[1] += 35;
        if (alphaWindow[1]>=255) alphaWindow[1] = 255;
    }
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        //currentUpdate = &vhpGameCore::playScreenSaver;
    }
    if ((alphaWindow[0]==255)&&(alphaWindow[1]==255)) {
        delay--;
        if (delay<=0) {
            cout << "Both user have trigered a window" << endl;
            cout << "Time A: " << time[0] << endl;
            cout << "Time B: " << time[1] << endl;
            alpha = 0;
            bool tie = false;
            // os dous acertaron
            if (ok[0]&&ok[1]) {
                // gaña o mais rápido
                if (time[0]<=time[1]) {
                    cout << "Player 0 was faster!" << endl;
                    winner = 0;
                    windowState[0][targetsShot] = wonW;
                    windowState[1][targetsShot] = lostW;
                } else {
                    cout << "Player 1 was faster!" << endl;
                    winner = 1;
                    windowState[0][targetsShot] = lostW;
                    windowState[1][targetsShot] = wonW;
                }
            } else {
                // acertou só 0
                if (ok[0]) {
                    cout << "Only 0 was succesful" << endl;
                    winner = 0;
                    windowState[0][targetsShot] = wonW;
                    windowState[1][targetsShot] = lostW;
                // acertou só 1
                } else if (ok[1]) {
                    cout << "Only 1 was succesful" << endl;
                    winner = 1;
                    windowState[0][targetsShot] = lostW;
                    windowState[1][targetsShot] = wonW;
                // no acertou ninguén
                } else {
                    cout << "Both players failed" << endl;
                    tie = true;
                }
            }
            if (tie) {
                hold[0] = false;
                hold[1] = false;
                currentUpdate = &vhpGameCore::showTie;
                currentTouchPressed = &vhpGameCore::touchPressedWinner;
                mensajeria.send("gamestate", 5);
            } else {
                hold[0] = false;
                hold[1] = false;
                points[winner]++;
                currentUpdate = &vhpGameCore::showWinner;
                currentTouchPressed = &vhpGameCore::touchPressedWinner;
                mensajeria.send("gamestate", 4);
            }
            
        }
    }
}

// Show Winner -----------------------------------
void vhpGameCore::showWinner(){
    // Reproduce el video y lo dibuja en el FBO
    video[1].update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    winnerBackground[winner].draw(0,0);
    ofSetColor(255, 255, 255);
    if (winner==0) {
        TTF.drawString("¡Rojo gana ronda 1!", 520, 180);
    } else {
        TTF.drawString("¡Azul gana ronda 1!", 520, 180);
    }
    winnerButton[0].draw(0,0);
    winnerButton[1].draw(0,0);
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        //currentUpdate = &vhpGameCore::playScreenSaver;
    }
}

// Show Winner -----------------------------------
void vhpGameCore::showFinalWinner(){
    // Reproduce el video y lo dibuja en el FBO
    video[1].update();
    fbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    video[1].draw(0, 0);
    ofSetColor(255,255,255,alpha);
    winnerBackground[winner].draw(0,0);
    ofSetColor(255, 255, 255);
    if (winner==0) {
        TTF.drawString("¡Rojo gana ronda 1!", 520, 180);
    } else {
        TTF.drawString("¡Azul gana ronda 1!", 520, 180);
    }
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        //currentUpdate = &vhpGameCore::playScreenSaver;
    }
}


// Show Tie -----------------------------------
void vhpGameCore::showTie(){
    // Reproduce el video y lo dibuja en el FBO
    video[0].update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    tie.draw(0,0);
    winnerButton[0].draw(0,0);
    winnerButton[1].draw(0,0);
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        //currentUpdate = &vhpGameCore::playScreenSaver;
    }
}

// Show Window Pattern -----------------------------------
void vhpGameCore::setWindowPattern(){
    initPattern();
    randomPattern();
    setTimeReference();
    currentUpdate = &vhpGameCore::showPattern;
    pWindow.setWindows(&purple, targetsPattern[0], &yellow, targetsPattern[1], &blue, targetsPattern[2], &green, targetsPattern[3]);
    pWindow.setFadeIn();
    currentTouchPressed = &vhpGameCore::touchPressedPattern;
}
void vhpGameCore::showPattern(){
    // pequeño delay antes de enseñar la ventana
    // Reproduce el video y lo dibuja en el FBO
    video[0].update();
    pWindow.update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    pWindow.draw();
    /*
    if (clicked[0]!=0) {
        ofSetColor(255,255,255,alphaWindow[0]);
        wClickA[clicked[0]].draw(0,0);
        alphaWindow[0] += 35;
        if (alphaWindow[0]>=255) alphaWindow[0] = 255;
    }
    if (clicked[1]!=0) {
        ofSetColor(255,255,255,alphaWindow[1]);
        wClickB[clicked[1]].draw(0,0);
        alphaWindow[1] += 35;
        if (alphaWindow[1]>=255) alphaWindow[1] = 255;
    }
     */
    
    
    if (hold[0]&&hold[1]) {
        delay--;
        if (delay<=0) {
            alpha = 0;
            bool tie = false;
            if (ok[0]&&ok[1]) {
                cout << "Both user have trigered a correct pattern" << endl;
                cout << "Time A: " << time[0] << endl;
                cout << "Time B: " << time[1] << endl;
                // gaña o mais rápido
                if (time[0]<=time[1]) {
                    cout << "Player 0 was faster!" << endl;
                    winner = 0;
                } else {
                    cout << "Player 1 was faster!" << endl;
                    winner = 1;
                }
                
            } else {
                // acertou só 0
                if (ok[0]) {
                    cout << "Only 0 was succesful" << endl;
                    winner = 0;
                    // acertou só 1
                } else if (ok[1]) {
                    cout << "Only 1 was succesful" << endl;
                    winner = 1;
                    // no acertou ninguén
                } else {
                    cout << "Both players failed" << endl;
                    tie = true;
                }
            }
            if (tie) {
                currentUpdate = &vhpGameCore::showTie;
                currentTouchPressed = &vhpGameCore::touchPressedWinner;
                
                mensajeria.send("gamestate", 5);
            } else {
                points[winner] += 3;
                if (points[0]>points[1]) {
                    winner = 0;
                } else {
                    winner = 1;
                }
                currentUpdate = &vhpGameCore::showFinalWinner;
                currentTouchPressed = &vhpGameCore::touchPressedPatternWinner;
                mensajeria.send("gamestate", 4);
            }
        }
        
    }
    
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    
}

// Eventos ------------------------------------------------------

void vhpGameCore::touchPressed(float & _x, float & _y){
    (*this.*currentTouchPressed)(_x, _y);
}

void vhpGameCore::touchPressedGame(float & _x, float & _y){
    cout << "MousePressed in game!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    
    // min y: 353, max y: 540
    cout << "mouse x: " << _y*3/scale << " mouse y: " << _x*3/scale << endl;
    if ((_y>=350)&&(_y<=545)) {
        // Player A
        if (_x<=960) {
            if (!hold[0]) {
                hold[0] = true;
                time[0] = ofGetElapsedTimeMillis();
                // Window 1
                if (_x<=171) {
                    clicked[0] = 0;
                    mensajeria.send("windowclick/a", 0);
                    cout << "Window 0" << endl;
                    
                    // Window 2
                } else if (_x<=316) {
                    clicked[0] = 1;
                    mensajeria.send("windowclick/a", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 3
                } else if (_x<=455) {
                    clicked[0] = 2;
                    mensajeria.send("windowclick/a", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 4
                } else if (_x<=587) {
                    clicked[0] = 3;
                    mensajeria.send("windowclick/a", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 5
                } else if (_x<=711) {
                    clicked[0] = 4;
                    mensajeria.send("windowclick/a", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 6
                } else if (_x<=832) {
                    clicked[0] = 5;
                    mensajeria.send("windowclick/a", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 7
                } else {
                    clicked[0] = 6;
                    mensajeria.send("windowclick/a", 6);
                    cout << "Window 6" << endl;
                    
                }
                if (clicked[0]==targetsShot) ok[0] = true;
                cout << "TargetShot " << targetsShot << endl;
            }
        // Player B
        } else {
            if (!hold[1]) {
                hold[1] = true;
                time[1] = ofGetElapsedTimeMillis();
                // Window 7
                if (_x<=1088) {
                    clicked[1] = 0;
                    mensajeria.send("windowclick/b", 0);
                    cout << "Window 0" << endl;
                    
                    // Window 6
                } else if (_x<=1210) {
                    clicked[1] = 1;
                    mensajeria.send("windowclick/b", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 5
                } else if (_x<=1334) {
                    clicked[1] = 2;
                    mensajeria.send("windowclick/b", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 4
                } else if (_x<=1466) {
                    clicked[1] = 3;
                    mensajeria.send("windowclick/b", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 3
                } else if (_x<=1603) {
                    clicked[1] = 4;
                    mensajeria.send("windowclick/b", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 2
                } else if (_x<=1748) {
                    clicked[1] = 5;
                    mensajeria.send("windowclick/b", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 1
                } else {
                    clicked[1] = 6;
                    mensajeria.send("windowclick/b", 6);
                    cout << "Window 6" << endl;
                    
                }
                if (clicked[1]==targetsShot) ok[1] = true;
                cout << "TargetShot " << targetsShot << endl;
            }
        }
    }
}
void vhpGameCore::touchPressedWinner(float & _x, float & _y){
 //
    cout << "MousePressed after game!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    
    // min y: 690, max y: 860
    cout << "mouse x: " << _y << " mouse y: " << _x << endl;
    if ((_y>=690)&&(_y<=860)) {
        // Player A
        if (_x<=960) {
            cout << "Is player A" << endl;
            if (!hold[0]) {
                cout << "Set next[0] true" << endl;
                next[0] = true;
                hold[0] = true;
                if (next[1]==true) {
                    cout << "next[1] is true" << endl;
                    currentRound++;
                    // cuatro primeras rondas
                    if (currentRound<nROUNDS) {
                        cout << "Next Round!" << endl;
                        initRound();
                        currentUpdate = &vhpGameCore::playReady;
                        currentTouchPressed = &vhpGameCore::touchPressedGame;
                        // última ronda
                    } else {
                        cout << "Last Round!" << endl;
                        setWindowPattern();
                    }
                }

            }
        
        // Player B
        } else {
            cout << "Is player B" << endl;
            if (!hold[1]) {
                cout << "Set next[1] true" << endl;
                next[1] = true;
                hold[1] = true;
                if (next[0]==true) {
                    cout << "next[0] is true" << endl;
                    currentRound++;
                    // cuatro primeras rondas
                    if (currentRound<nROUNDS) {
                        cout << "Next Round!" << endl;
                        initRound();
                        currentUpdate = &vhpGameCore::playReady;
                        currentTouchPressed = &vhpGameCore::touchPressedGame;
                        // última ronda
                    } else {
                        cout << "Last Round!" << endl;
                        setWindowPattern();
                    }
                }
            }
        }
    }
}
void vhpGameCore::touchPressedPattern(float & _x, float & _y){
    cout << "MousePressed during pattern!" << endl;
    int n[2] ={0,0};
    for (int i = 0; i<2; i++) {
        if(registeredPattern[i][2]!=7) {
            n[i] = 3;
        } else if(registeredPattern[i][1]!=7) {
            n[i] = 2;
        } else if(registeredPattern[i][0]!=7) {
            n[i] = 1;
        }
    }
    
    // min y: 353, max y: 540
    cout << "click: " << n[0] << " mouse x: " << _y*3/scale << " mouse y: " << _x*3/scale << endl;
    
    if ((_y>=350)&&(_y<=545)) {
        // Player A
        if (_x<=960) {
            if (!hold[0]) {
                // Window 1
                if (_x<=171) {
                    registeredPattern[0][n[0]] = 0;
                    mensajeria.send("windowclick/a", 0);
                    cout << "Window 0" << endl;
                    
                    // Window 2
                } else if (_x<=316) {
                    registeredPattern[0][n[0]] = 1;
                    mensajeria.send("windowclick/a", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 3
                } else if (_x<=455) {
                    registeredPattern[0][n[0]] = 2;
                    mensajeria.send("windowclick/a", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 4
                } else if (_x<=587) {
                    registeredPattern[0][n[0]] = 3;
                    mensajeria.send("windowclick/a", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 5
                } else if (_x<=711) {
                    registeredPattern[0][n[0]] = 4;
                    mensajeria.send("windowclick/a", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 6
                } else if (_x<=832) {
                    registeredPattern[0][n[0]] = 5;
                    mensajeria.send("windowclick/a", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 7
                } else {
                    registeredPattern[0][n[0]] = 6;
                    mensajeria.send("windowclick/a", 6);
                    cout << "Window 6" << endl;
                }
                if ((n[0]==3)&&(registeredPattern[0][3]!=7)) {
                    cout << "Lets check" << endl;
                    time[0] = ofGetElapsedTimeMillis();
                    hold[0] = true;
                    if ((targetsPattern[0]==registeredPattern[0][0])&&(targetsPattern[1]==registeredPattern[0][1])&&(targetsPattern[2]==registeredPattern[0][2])&&(targetsPattern[3]==registeredPattern[0][3])) {
                        ok[0] = true;
                        cout << "Player 0 has succeded!" << endl;
                    }
                }
            }
        // Player B
        } else {
            if (!hold[1]) {
                // Window 7
                if (_x<=1088) {
                    registeredPattern[1][n[1]] = 6;
                    mensajeria.send("windowclick/b", 6);
                    cout << "Window 6" << endl;
                    
                    // Window 6
                } else if (_x<=1210) {
                    registeredPattern[1][n[1]] = 5;
                    mensajeria.send("windowclick/b", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 5
                } else if (_x<=1334) {
                    registeredPattern[1][n[1]] = 4;
                    mensajeria.send("windowclick/b", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 4
                } else if (_x<=1466) {
                    registeredPattern[1][n[1]] = 3;
                    mensajeria.send("windowclick/b", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 3
                } else if (_x<=1603) {
                    registeredPattern[1][n[1]] = 2;
                    mensajeria.send("windowclick/b", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 2
                } else if (_x<=1748) {
                    registeredPattern[1][n[1]] = 1;
                    mensajeria.send("windowclick/b", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 1
                } else {
                    registeredPattern[1][n[1]] = 0;
                    mensajeria.send("windowclick/b", 0);
                    cout << "Window 0" << endl;
                    
                }
                
                if ((n[1]==3)&&(registeredPattern[1][3]!=7)) {
                    cout << "Lets check" << endl;
                    time[1] = ofGetElapsedTimeMillis();
                    hold[1] = true;
                    if ((targetsPattern[0]==registeredPattern[1][0])&&(targetsPattern[1]==registeredPattern[1][1])&&(targetsPattern[2]==registeredPattern[1][2])&&(targetsPattern[3]==registeredPattern[1][3])) {
                        ok[1] = true;
                        cout << "Player 1 has succeded!" << endl;
                    }
                }
                
            }
        }
    }
}
void vhpGameCore::touchPressedPatternWinner(float & _x, float & _y){
    //
    cout << "MousePressed after Pattern!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    
    // min y: 690, max y: 860
    cout << "mouse x: " << _y << " mouse y: " << _x << endl;
    if ((_y>=690)&&(_y<=860)) {
        // Player A
        if (_x<=960) {
            next[0] = true;
            if (next[1]==true) {
                            }
            // Player B
        } else {
            next[1] = true;
            if (next[0]==true) {
                
            }
        }
    }
}

int vhpGameCore::randomWindow(){
    vector<int> nums;
    for (int i=0; i<nWINDOWS; i++) {
        if (windowState[0][i]==pendingW) {
            nums.push_back(i);
        }
    }
    return nums[ceil(ofRandom(nums.size()-1))];
}
void vhpGameCore::randomPattern(){
    int first = ceil(ofRandom(nWINDOWS-1));
    cout << "firt =" << first << endl;
    vector<int> nums;
    for (int i=0; i<nWINDOWS; i++) {
        if (i==first) {
            targetsPattern[0]= i;
            cout << "targetsPattern[0]=" << i << endl;
        } else {
            nums.push_back(i);
        }
    }
    int second = ceil(ofRandom(nums.size()-1));
    cout << "second =" << second << endl;
    vector<int> snums;
    for (int i=0; i<nums.size(); i++) {
        if (i==second) {
            targetsPattern[1]= nums[i];
            cout << "targetsPattern[1]=" << nums[i] << endl;
        } else {
            snums.push_back(nums[i]);
        }
    }
    int third = ceil(ofRandom(snums.size()-1));
    cout << "third =" << third << endl;
    vector<int> tnums;
    for (int i=0; i<snums.size(); i++) {
        if (i==third) {
            targetsPattern[2]= snums[i];
            cout << "targetsPattern[2]=" << snums[i] << endl;
        } else {
            tnums.push_back(snums[i]);
        }
    }
    
    int fourth = ceil(ofRandom(tnums.size()-1));
    cout << "fourth =" << fourth << endl;
    for (int i=0; i<tnums.size(); i++) {
        if (i==fourth) {
            targetsPattern[3]= tnums[i];
            cout << "targetsPattern[3]=" << tnums[i] << endl;
        }
    }
}

void vhpGameCore::setTimeReference(){
    tRef = ofGetElapsedTimef();
}

float vhpGameCore::getElapsedtime(){
    return ofGetElapsedTimef() - tRef;
}


ofEvent <int> vhpGameCore::onClick;