#include "vhpPlayerMenu.h"

// Constructor -------------------------------------------------

vhpPlayerMenu::vhpPlayerMenu():scale(1.0),state(MENU),selected(0){

}

vhpPlayerMenu::~vhpPlayerMenu(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------

void vhpPlayerMenu::setup(vhpPmThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene){
    
    // Inicializar el hilo de control
    controller = _controller;
    controller->setup(&video, 0.0, 9.0/57.0);
    
    // Añadir los vídeos desde el documento xml de settings
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
    targetScene = _targetScene;     // STANDBY
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

void vhpPlayerMenu::start(){
    cout << "Player Menu start();" << endl;
    if(!registerEvents) {
        ofRegisterMouseEvents(this); // this will enable our circle class to listen to the mouse events.
        registerEvents = true;
    }
    controller->start();
}

void vhpPlayerMenu::stop(){
    if(registerEvents) {
        ofUnregisterMouseEvents(this); // disable litening to mous events.
        registerEvents = false;
    }
    controller->stop();
}

// Dibujado y actualización variables --------------------------
void vhpPlayerMenu::update(){
    (*this.*currentUpdate)();
}

//--------------------------------------------------------------
void vhpPlayerMenu::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Reproducir o detener la escena modificando currentUpdate ----

void vhpPlayerMenu::play(){
    currentUpdate = &vhpPlayerMenu::playPlayerMenu;
    video.play();
}

void vhpPlayerMenu::pause(){
    currentUpdate = &vhpPlayerMenu::pausePlayerMenu;
    video.stop();
}

// Procesado y actualización -----------------------------------

void vhpPlayerMenu::playPlayerMenu(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    video.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

void vhpPlayerMenu::pausePlayerMenu(){
    // Al no hacer nada mantiene el FBO con el último fotograma reproducido
    
}

void vhpPlayerMenu::loopPlayerMenu(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    video.setPosition(_pos);
}


// Utilidades ---------------------------------------------------

void vhpPlayerMenu::alert(int _e){
    cout << " Event detected " << _e << endl;
}

float vhpPlayerMenu::getPosition(){
    return video.getPosition();
}

// Eventos ------------------------------------------------------

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
                        controller->reset(12.0/57.0, 16.0/57.0);
                        break;
                    case ONEPLAYER:
                        if (y>=900) {
                            selected = 1;
                            ofNotifyEvent(playersNumber, targetScene);
                            cout << "ONEPLAYER confirmed!" << endl;
                        }
                        break;
                    case TWOPLAYERS:
                        video.setPosition(29.0/57.0);
                        controller->fadeReset(10.0/57.0, 30.0/57.0, 12.0/57.0, 16.0/57.0);
                        break;
                    case FOURPLAYERS:
                        video.setPosition(41.0/57.0);
                        controller->fadeReset(10.0/57.0, 42.0/57.0, 12.0/57.0, 16.0/57.0);
                        break;
                }
                state = ONEPLAYER;
            } else if (x<=1130) {
                cout << "2 xogadores" << endl;
                switch (state) {
                    case MENU:
                        video.setPosition(22.0/57.0);
                        controller->reset(24.0/57.0, 28.0/57.0);
                        break;
                    case ONEPLAYER:
                        video.setPosition(17.0/57.0);
                        controller->fadeReset(22.0/57.0, 18.0/57.0, 24.0/57.0, 28.0/57.0);
                        break;
                    case TWOPLAYERS:
                        if (y>=900) {
                            selected = 2;
                            ofNotifyEvent(playersNumber, targetScene);
                            cout << "TWOPLAYERS confirmed!" << endl;
                        }
                        break;
                    case FOURPLAYERS:
                        video.setPosition(41.0/57.0);
                        controller->fadeReset(22.0/57.0, 42.0/57.0, 24.0/57.0, 28.0/57.0);
                        break;
                }
                state = TWOPLAYERS;
            } else {
                cout << "4 xogadores" << endl;
                switch (state) {
                    case MENU:
                        video.setPosition(34.0/57.0);
                        controller->reset(36.0/57.0, 40.0/57.0);
                        break;
                    case ONEPLAYER:
                        video.setPosition(17.0/57.0);
                        controller->fadeReset(34.0/57.0, 18.0/57.0, 36.0/57.0, 40.0/57.0);
                        break;
                    case TWOPLAYERS:
                        video.setPosition(29.0/57.0);
                        controller->fadeReset(34.0/57.0, 30.0/57.0, 36.0/57.0, 40.0/57.0);
                        break;
                    case FOURPLAYERS:
                        if (y>=900) {
                            selected = 4;
                            ofNotifyEvent(playersNumber, targetScene);
                            cout << "FOURPLAYERS confirmed!" << endl;
                        }
                        break;
                }
                state = FOURPLAYERS;
            }
        } else {
            cout << "fora" << endl;
            switch (state) {
                case ONEPLAYER:
                    video.setPosition(17.0/57.0);
                    controller->fadeReset(0.0, 18.0/57.0, 0.0, 9.0/57.0);
                    break;
                case TWOPLAYERS:
                    video.setPosition(29.0/57.0);
                    controller->fadeReset(0.0, 30.0/57.0, 0.0, 9.0/57.0);
                    break;
                case FOURPLAYERS:
                    video.setPosition(41.0/57.0);
                    controller->fadeReset(0.0, 42.0/57.0, 0.0, 9.0/57.0);
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
                    controller->fadeReset(0.0, 18.0/57.0, 0.0, 9.0/57.0);
                    break;
                case TWOPLAYERS:
                    video.setPosition(29.0/57.0);
                    controller->fadeReset(0.0, 30.0/57.0, 0.0, 9.0/57.0);
                    break;
                case FOURPLAYERS:
                    video.setPosition(41.0/57.0);
                    controller->fadeReset(0.0, 42.0/57.0, 0.0, 9.0/57.0);
                    break;
            }
            state = MENU;
        }
    }
}

void vhpPlayerMenu::mouseScrolled(ofMouseEventArgs & _args){}
void vhpPlayerMenu::mouseEntered(ofMouseEventArgs & _args){}
void vhpPlayerMenu::mouseExited(ofMouseEventArgs & _args){}

ofEvent <int> vhpPlayerMenu::playersNumber;
