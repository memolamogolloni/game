#include "vhpStandby.h"

// Constructor -------------------------------------------------

vhpStandby::vhpStandby(){

}

vhpStandby::~vhpStandby(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------
void vhpStandby::setup(vhpStandbyThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene){
    
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
    currentScene = _currentScene;   // SCREENSAVER
    targetScene = _targetScene;     // PLAYERMENU
    width = video.getWidth();
    height = video.getHeight();
    fbo.allocate(width, height, GL_RGBA);
    
    // clean FBO
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
}

// Comenzar e interrumpir los hilos y listeners de la escena ---

void vhpStandby::start(){
    if(!registerEvents) {
        // Esto permite registrar los eventos del ratón sin necesidad de crear eventos propios
        ofRegisterMouseEvents(this);
        registerEvents = true;
    }
}

void vhpStandby::stop(){
    if(registerEvents) {
        // si el objeto no funciona desactivamos el registro de los eventos del ratón
        ofUnregisterMouseEvents(this);
        registerEvents = false;
    }
}

// Dibujado y actualización variables --------------------------

void vhpStandby::update(){
    (*this.*currentUpdate)();
}

void vhpStandby::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Reproducir o detener la escena modificando currentUpdate ----

void vhpStandby::play(){
    currentUpdate = &vhpStandby::playScreenSaver;
    video.play();
}

void vhpStandby::pause(){
    currentUpdate = &vhpStandby::pause;
    video.stop();
}

// Procesado y actualización -----------------------------------

void vhpStandby::playScreenSaver(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    video.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

void vhpStandby::pauseScreenSaver(){
    // Al no hacer nada mantiene el FBO con el último fotograma reproducido
}

void vhpStandby::loopScreenSaver(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    video.setPosition(_pos);
}

// Eventos ------------------------------------------------------

void vhpStandby::mouseMoved(ofMouseEventArgs & _args){}
void vhpStandby::mouseDragged(ofMouseEventArgs & _args){}
void vhpStandby::mousePressed(ofMouseEventArgs & _args){}

void vhpStandby::mouseReleased(ofMouseEventArgs & _args){
    cout << "Button active!" << endl;
    ofNotifyEvent(onClick, targetScene);
}

void vhpStandby::mouseScrolled(ofMouseEventArgs & _args){}
void vhpStandby::mouseEntered(ofMouseEventArgs & _args){}
void vhpStandby::mouseExited(ofMouseEventArgs & _args){}

ofEvent <int> vhpStandby::onClick;
