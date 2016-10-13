#include "vhpGameCore.h"

// Constructor -------------------------------------------------

vhpGameCore::vhpGameCore(){

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

// Reproducir o detener la escena modificando currentUpdate ----

void vhpGameCore::play(){
    currentUpdate = &vhpGameCore::playScreenSaver;
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
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    video.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

void vhpGameCore::pauseScreenSaver(){
    // Al no hacer nada mantiene el FBO con el último fotograma reproducido
}

void vhpGameCore::loopScreenSaver(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    video.setPosition(_pos);
}

// Eventos ------------------------------------------------------

void vhpGameCore::mouseMoved(ofMouseEventArgs & _args){}
void vhpGameCore::mouseDragged(ofMouseEventArgs & _args){}
void vhpGameCore::mousePressed(ofMouseEventArgs & _args){}

void vhpGameCore::mouseReleased(ofMouseEventArgs & _args){
    cout << "Button active!" << endl;
    ofNotifyEvent(onClick, targetScene);
}

void vhpGameCore::mouseScrolled(ofMouseEventArgs & _args){}
void vhpGameCore::mouseEntered(ofMouseEventArgs & _args){}
void vhpGameCore::mouseExited(ofMouseEventArgs & _args){}

ofEvent <int> vhpGameCore::onClick;
