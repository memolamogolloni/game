#include "vhpGameCore.h"

// Constructor -------------------------------------------------

vhpGameCore::vhpGameCore(): scale(1.0), currentRound(1){

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
    winner = 0;
}

// Comenzar e interrumpir los hilos y listeners de la escena ---

void vhpGameCore::start(){
    if(!registerEvents) {
        // Esto permite registrar los eventos del ratón sin necesidad de crear eventos propios
        ofRegisterMouseEvents(this);
        registerEvents = true;
    }
}

void vhpGameCore::stop(){
    if(registerEvents) {
        // si el objeto no funciona desactivamos el registro de los eventos del ratón
        ofUnregisterMouseEvents(this);
        registerEvents = false;
    }
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
    score[0].draw(260, 925, 700, 81);
    score[1].draw(260+700, 925, 700, 81);
    ofSetColor(255,255,255);
    ofSetLineWidth(1);
    ofDrawLine(260+700, 925+16, 260+700, 925+16+49);
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
        windowA[clicked[0]-1].draw(0,0);
        alphaWindow[0] += 35;
        if (alphaWindow[0]>=255) alphaWindow[0] = 255;
    }
    if (clicked[1]!=0) {
        ofSetColor(255,255,255,alphaWindow[1]);
        windowB[clicked[1]-1].draw(0,0);
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
        cout << "Both user have trigered a window" << endl;
        cout << "Time A: " << time[0] << endl;
        cout << "Time B: " << time[1] << endl;
        alpha = 0;
        // os dous acertaron
        if (ok[0]&&ok[1]) {
            // gaña o mais rápido
            if (time[0]<=time[1]) {
                winner = 0;
            } else {
                winner = 1;
            }
        } else {
            // acertou só 0
            if (ok[0]) {
                winner = 0;
                
            // acertou só 1
            } else if (ok[1]) {
                winner = 1;
            
            // no acertou ninguén
            } else {
                
            }
        }
        currentUpdate = &vhpGameCore::showWinner;
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

void vhpGameCore::mouseMoved(ofMouseEventArgs & _args){}
void vhpGameCore::mouseDragged(ofMouseEventArgs & _args){}
void vhpGameCore::mousePressed(ofMouseEventArgs & _args){
    cout << "MousePressed in game!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    
    // min y: 353, max y: 540
    cout << "mouse x: " << _args.y*3/scale << " mouse y: " << _args.x*3/scale << endl;
    float y = _args.y*3/scale;
    if ((y>=350)&&(y<=545)) {
        float x =  _args.x*3/scale;
        // Player A
        if (x<=960) {
            if (!hold[0]) {
                hold[0] = true;
                time[0] = ofGetElapsedTimeMillis();
                // Window 1
                if (x<=171) {
                    clicked[0] = 1;
                    cout << "Window 1" << endl;
                    
                // Window 2
                } else if (x<=316) {
                    clicked[0] = 2;
                    cout << "Window 2" << endl;
                    
                // Window 3
                } else if (x<=455) {
                    clicked[0] = 3;
                    cout << "Window 3" << endl;
                    
                // Window 4
                } else if (x<=587) {
                    clicked[0] = 4;
                    cout << "Window 4" << endl;
                    
                // Window 5
                } else if (x<=711) {
                    clicked[0] = 5;
                    cout << "Window 5" << endl;
                    
                // Window 6
                } else if (x<=832) {
                    clicked[0] = 6;
                    cout << "Window 6" << endl;
                    
                // Window 7
                } else {
                    clicked[0] = 7;
                    cout << "Window 7" << endl;
                    
                }
                if (clicked[0]==targetsShot[0]) ok[0] = true;
            }
        // Player B
        } else {
            if (!hold[1]) {
                hold[1] = true;
                time[1] = ofGetElapsedTimeMillis();
                // Window 7
                if (x<=1088) {
                    clicked[1] = 7;
                    cout << "Window 7" << endl;
                    
                // Window 6
                } else if (x<=1210) {
                    clicked[1] = 6;
                    cout << "Window 6" << endl;
                    
                // Window 5
                } else if (x<=1334) {
                    clicked[1] = 5;
                    cout << "Window 5" << endl;
                    
                // Window 4
                } else if (x<=1466) {
                    clicked[1] = 4;
                    cout << "Window 4" << endl;
                    
                // Window 3
                } else if (x<=1603) {
                    clicked[1] = 3;
                    cout << "Window 3" << endl;
                    
                // Window 2
                } else if (x<=1748) {
                    clicked[1] = 2;
                    cout << "Window 2" << endl;
                
                // Window 1
                } else {
                    clicked[1] = 1;
                    cout << "Window 1" << endl;
                    
                }
                if (clicked[1]==targetsShot[1]) ok[1] = true;
            }
        }
    }
}

void vhpGameCore::mouseReleased(ofMouseEventArgs & _args){
    cout << "Button released in game!" << endl;
    //ofNotifyEvent(onClick, targetScene);
}

void vhpGameCore::mouseScrolled(ofMouseEventArgs & _args){}
void vhpGameCore::mouseEntered(ofMouseEventArgs & _args){}
void vhpGameCore::mouseExited(ofMouseEventArgs & _args){}

ofEvent <int> vhpGameCore::onClick;
