#include "vhpGameCore.h"

// Constructor -------------------------------------------------

vhpGameCore::vhpGameCore(): scale(1.0){

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
        if(num > 0) video.load(_videoList.getValue("VIDEO", "", 0));
        _videoList.popTag();
    }
    building.load("images/fondo-edificio.png");
    score[0].load("images/red-bar.png");
    score[1].load("images/blue-bar.png");
    ready.load("images/ready.png");
    steady.load("images/steady.png");
    winnerBackground[0].load("images/red-wins.png");
    winnerBackground[1].load("images/blue-wins.png");
    winnerButton[0].load("images/red-wins-button.png");
    winnerButton[1].load("images/blue-wins-button.png");
    go.load("images/go.png");
    for (int i = 0; i<nWINDOWS; i++) {
        windowA[i].load("images/1P-Boton-0"+ ofToString(i) +".png");
        windowB[i].load("images/2P-Boton-0"+ ofToString(i) +".png");
        windowShowA[i].load("images/window_ashow_0"+ ofToString(i) +".png");
        windowShowB[i].load("images/window_bshow_0"+ ofToString(i) +".png");
    }
    alpha = 0;
    alpha_increment = 5;
    
    TTF.load("fonts/TitilliumWeb-Light.ttf", 70);
    
    // Inicializar las variables
    currentScene = _currentScene;   // SCREENSAVER
    targetScene = _targetScene;     // PLAYERMENU
    width = video.getWidth();
    height = video.getHeight();
    
    cout << " video width: " << width << " height: " << height << endl;
    
    fbo.allocate(width, height, GL_RGBA);
    
    // clean FBO
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
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
    holdSteady = ceil(ofRandom(3));
    targetsShot[0]= ceil(ofRandom(nWINDOWS-1));
    targetsShot[1]= ceil(ofRandom(nWINDOWS-1));
    alpha = 0;
    alphaWindow[0] = 0;
    alphaWindow[1] = 0;
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
    ofDrawLine(260 + widthA, 925+16, 260 + widthA, 925+16+49);
    ofPopStyle();
}

void vhpGameCore::drawGame(){
    ofPushStyle();
    ofSetColor(255, 255, 255);
    video.draw(0, 0);
    building.draw(0, 0);
    drawScore();
    ofPopStyle();
}

// Reproducir o detener la escena modificando currentUpdate ----

void vhpGameCore::play(){
    initRound();
    currentUpdate = &vhpGameCore::playReady;
    currentTouchPressed = &vhpGameCore::touchPressedGame;
    video.play();
}

void vhpGameCore::pause(){
    currentUpdate = &vhpGameCore::pause;
    video.stop();
}

// Procesado y actualización -----------------------------------

void vhpGameCore::playScreenSaver(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
    fbo.begin();
    drawGame();
    fbo.end();
}

void vhpGameCore::pauseScreenSaver(){
    // Al no hacer nada mantiene el FBO con el último fotograma reproducido
}

void vhpGameCore::loopScreenSaver(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    video.setPosition(_pos);
}

// Ready -----------------------------------
void vhpGameCore::playReady(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
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
    }
}

// Steady -----------------------------------
void vhpGameCore::playSteady(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
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
        }
        holdSteady --;
    }
}
// Go -----------------------------------
void vhpGameCore::playGo(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
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
    }
}

// Show Window -----------------------------------
void vhpGameCore::showWindow(){
    // pequeño delay antes de enseñar la ventana
    // Reproduce el video y lo dibuja en el FBO
    video.update();
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    windowShowA[targetsShot[0]].draw(0,0);
    windowShowB[targetsShot[1]].draw(0,0);
    if (clicked[0]!=0) {
        ofSetColor(255,255,255,alphaWindow[0]);
        windowA[clicked[0]].draw(0,0);
        alphaWindow[0] += 35;
        if (alphaWindow[0]>=255) alphaWindow[0] = 255;
    }
    if (clicked[1]!=0) {
        ofSetColor(255,255,255,alphaWindow[1]);
        windowB[clicked[1]].draw(0,0);
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
            // os dous acertaron
            if (ok[0]&&ok[1]) {
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
                    
                }
            }
            points[winner]++;
            currentUpdate = &vhpGameCore::showWinner;
            currentTouchPressed = &vhpGameCore::touchPressedWinner;
        }
    }
}

// Show Winner -----------------------------------
void vhpGameCore::showWinner(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
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
                    cout << "Window 0" << endl;
                    
                    // Window 2
                } else if (_x<=316) {
                    clicked[0] = 1;
                    cout << "Window 1" << endl;
                    
                    // Window 3
                } else if (_x<=455) {
                    clicked[0] = 2;
                    cout << "Window 2" << endl;
                    
                    // Window 4
                } else if (_x<=587) {
                    clicked[0] = 3;
                    cout << "Window 3" << endl;
                    
                    // Window 5
                } else if (_x<=711) {
                    clicked[0] = 4;
                    cout << "Window 4" << endl;
                    
                    // Window 6
                } else if (_x<=832) {
                    clicked[0] = 5;
                    cout << "Window 5" << endl;
                    
                    // Window 7
                } else {
                    clicked[0] = 6;
                    cout << "Window 6" << endl;
                    
                }
                if (clicked[0]==targetsShot[0]) ok[0] = true;
                cout << "TargetShot " << targetsShot[0] << endl;
            }
        // Player B
        } else {
            if (!hold[1]) {
                hold[1] = true;
                time[1] = ofGetElapsedTimeMillis();
                // Window 7
                if (_x<=1088) {
                    clicked[1] = 6;
                    cout << "Window 6" << endl;
                    
                    // Window 6
                } else if (_x<=1210) {
                    clicked[1] = 5;
                    cout << "Window 5" << endl;
                    
                    // Window 5
                } else if (_x<=1334) {
                    clicked[1] = 4;
                    cout << "Window 4" << endl;
                    
                    // Window 4
                } else if (_x<=1466) {
                    clicked[1] = 3;
                    cout << "Window 3" << endl;
                    
                    // Window 3
                } else if (_x<=1603) {
                    clicked[1] = 2;
                    cout << "Window 2" << endl;
                    
                    // Window 2
                } else if (_x<=1748) {
                    clicked[1] = 1;
                    cout << "Window 1" << endl;
                    
                    // Window 1
                } else {
                    clicked[1] = 0;
                    cout << "Window 0" << endl;
                    
                }
                if (clicked[1]==targetsShot[1]) ok[1] = true;
                cout << "TargetShot " << targetsShot[1] << endl;
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
            next[0] = true;
            if (next[1]) {
                cout << "Next Round!" << endl;
                initRound();
                currentRound++;
                // cuatro primeras rondas
                if (currentRound<5) {
                    currentUpdate = &vhpGameCore::playReady;
                    currentTouchPressed = &vhpGameCore::touchPressedGame;
                // última ronda
                } else {
                    
                }
            }
        // Player B
        } else {
            next[1] = true;
            if (next[0]) {
                cout << "Next Round!" << endl;
                initRound();
                currentRound++;
                // cuatro primeras rondas
                if (currentRound<5) {
                    currentUpdate = &vhpGameCore::playReady;
                    currentTouchPressed = &vhpGameCore::touchPressedGame;
                
                // última ronda
                } else {
                    
                }
            }
        }
    }
}

ofEvent <int> vhpGameCore::onClick;
