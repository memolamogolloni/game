#include "vhpLevelMenu.h"

// Constructor -------------------------------------------------

vhpLevelMenu::vhpLevelMenu():state(MENUNIVEL),selected(0){

}

vhpLevelMenu::~vhpLevelMenu(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------

void vhpLevelMenu::setup(vhpLmThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene){
    
    // Inicializar el hilo de control
    controller = _controller;
    controller->setup(&video, 0.0, 9.0/57.0);
    
    // A–adir los v’deos desde el documento xml de settings
    int n = _videoList.getNumTags(_videoTag +":VIDEO");
    if(n > 0){
        _videoList.pushTag(_videoTag, n-1);
        int num = _videoList.getNumTags("VIDEO");
        cout << num << " videos in " << _videoTag << " " << _videoList.getValue("VIDEO", "", 0) << endl;
        if(num > 0) video.load(_videoList.getValue("VIDEO", "", 0));
        _videoList.popTag();
    }
    
    // Inicializar las variables
    currentScene = _currentScene;   // PLAYERMENU
    targetScene = _targetScene;     // GAME
    width = video.getWidth();
    height = video.getHeight();
    
    cout << " video width: " << width << " height: " << height << endl;
    
    fbo.allocate(width, height, GL_RGBA);
    
    // clean FBO
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
}

// Comenzar e interrumpir los hilos y listeners de la escena ---

void vhpLevelMenu::start(){
    controller->start();
}

void vhpLevelMenu::stop(){
    controller->stop();
}

// Dibujado y actualizaci—n variables --------------------------
void vhpLevelMenu::update(){
    (*this.*currentUpdate)();
}

//--------------------------------------------------------------
void vhpLevelMenu::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Reproducir o detener la escena modificando currentUpdate ----

void vhpLevelMenu::play(){
    currentUpdate = &vhpLevelMenu::playPlayerMenu;
    video.play();
}

void vhpLevelMenu::pause(){
    currentUpdate = &vhpLevelMenu::pausePlayerMenu;
    video.stop();
}

// Procesado y actualizaci—n -----------------------------------

void vhpLevelMenu::playPlayerMenu(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    video.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

void vhpLevelMenu::pausePlayerMenu(){
    // Al no hacer nada mantiene el FBO con el œltimo fotograma reproducido
    
}

void vhpLevelMenu::loopPlayerMenu(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    video.setPosition(_pos);
}


// Utilidades ---------------------------------------------------

void vhpLevelMenu::alert(int _e){
    cout << " Event detected " << _e << endl;
}

float vhpLevelMenu::getPosition(){
    return video.getPosition();
}

// Eventos ------------------------------------------------------

void vhpLevelMenu::touchPressed(float _x, float _y){
    cout << "Menu active!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    // min x: 520, min y: 182 408 538 630 160
    cout << "mouse x: " << _x << " mouse y: " << _y << endl;
    if (_y>=520) {
        if ((_x>=182)&&(_x<=1760)) {
            if (_x<=592) {
                cout << "DESTREZA" << endl;
                switch (state) {
                    case MENUNIVEL:
                        video.setPosition(10.0/57.0);
                        controller->reset(12.0/57.0, 16.0/57.0);
                        break;
                    case DESTREZA:
                        if (_y>=900) {
                            selected = 1;
                            ofNotifyEvent(levelSelection, targetScene);
                            cout << "DESTREZA confirmed!" << endl;
                        }
                        break;
                    case ESPIRITUALIDAD:
                        video.setPosition(29.0/57.0);
                        controller->fadeReset(10.0/57.0, 30.0/57.0, 12.0/57.0, 16.0/57.0);
                        break;
                    case ORATORIA:
                        video.setPosition(41.0/57.0);
                        controller->fadeReset(10.0/57.0, 42.0/57.0, 12.0/57.0, 16.0/57.0);
                        break;
                }
                state = DESTREZA;
            } else if (_x<=1130) {
                cout << "ESPIRITUALIDAD" << endl;
                switch (state) {
                    case MENUNIVEL:
                        video.setPosition(22.0/57.0);
                        controller->reset(24.0/57.0, 28.0/57.0);
                        break;
                    case DESTREZA:
                        video.setPosition(17.0/57.0);
                        controller->fadeReset(22.0/57.0, 18.0/57.0, 24.0/57.0, 28.0/57.0);
                        break;
                    case ESPIRITUALIDAD:
                        if (_y>=900) {
                            selected = 2;
                            ofNotifyEvent(levelSelection, targetScene);
                            cout << "ESPIRITUALIDAD confirmed!" << endl;
                        }
                        break;
                    case ORATORIA:
                        video.setPosition(41.0/57.0);
                        controller->fadeReset(22.0/57.0, 42.0/57.0, 24.0/57.0, 28.0/57.0);
                        break;
                }
                state = ESPIRITUALIDAD;
            } else {
                cout << "ORATORIA" << endl;
                switch (state) {
                    case MENUNIVEL:
                        video.setPosition(34.0/57.0);
                        controller->reset(36.0/57.0, 40.0/57.0);
                        break;
                    case DESTREZA:
                        video.setPosition(17.0/57.0);
                        controller->fadeReset(34.0/57.0, 18.0/57.0, 36.0/57.0, 40.0/57.0);
                        break;
                    case ESPIRITUALIDAD:
                        video.setPosition(29.0/57.0);
                        controller->fadeReset(34.0/57.0, 30.0/57.0, 36.0/57.0, 40.0/57.0);
                        break;
                    case ORATORIA:
                        if (_y>=900) {
                            selected = 4;
                            ofNotifyEvent(levelSelection, targetScene);
                            cout << "ORATORIA confirmed!" << endl;
                        }
                        break;
                }
                state = ORATORIA;
            }
        } else {
            cout << "fora" << endl;
            switch (state) {
                case DESTREZA:
                    video.setPosition(17.0/57.0);
                    controller->fadeReset(0.0, 18.0/57.0, 0.0, 9.0/57.0);
                    break;
                case ESPIRITUALIDAD:
                    video.setPosition(29.0/57.0);
                    controller->fadeReset(0.0, 30.0/57.0, 0.0, 9.0/57.0);
                    break;
                case ORATORIA:
                    video.setPosition(41.0/57.0);
                    controller->fadeReset(0.0, 42.0/57.0, 0.0, 9.0/57.0);
                    break;
            }
            state = MENUNIVEL;
        }
    } else {
        if (state != MENUNIVEL) {
            cout << "fora" << endl;
            switch (state) {
                case DESTREZA:
                    video.setPosition(17.0/57.0);
                    controller->fadeReset(0.0, 18.0/57.0, 0.0, 9.0/57.0);
                    break;
                case ESPIRITUALIDAD:
                    video.setPosition(29.0/57.0);
                    controller->fadeReset(0.0, 30.0/57.0, 0.0, 9.0/57.0);
                    break;
                case ORATORIA:
                    video.setPosition(41.0/57.0);
                    controller->fadeReset(0.0, 42.0/57.0, 0.0, 9.0/57.0);
                    break;
            }
            state = MENUNIVEL;
        }
    }
}

ofEvent <int> vhpLevelMenu::levelSelection;
