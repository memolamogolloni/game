#include "vhpScreenSaver.h"

// Constructor -------------------------------------------------

vhpScreenSaver::vhpScreenSaver(){

}

vhpScreenSaver::~vhpScreenSaver(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------
void vhpScreenSaver::setup(ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene){
    
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

void vhpScreenSaver::start(){
    cout << "Screen Saver start();" << endl;
}

void vhpScreenSaver::stop(){
}

// Dibujado y actualización variables --------------------------

void vhpScreenSaver::update(){
    (*this.*currentUpdate)();
}

void vhpScreenSaver::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Reproducir o detener la escena modificando currentUpdate ----

void vhpScreenSaver::play(){
    currentUpdate = &vhpScreenSaver::playScreenSaver;
    video.play();
}

void vhpScreenSaver::pause(){
    currentUpdate = &vhpScreenSaver::pause;
    video.stop();
}

// Procesado y actualización -----------------------------------

void vhpScreenSaver::playScreenSaver(){
    // Reproduce el video y lo dibuja en el FBO
    video.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    video.draw(0, 0);
    fbo.end();
    ofPopStyle();
}

void vhpScreenSaver::pauseScreenSaver(){
    // Al no hacer nada mantiene el FBO con el último fotograma reproducido
}

void vhpScreenSaver::loopScreenSaver(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    video.setPosition(_pos);
}

// Eventos ------------------------------------------------------
void vhpScreenSaver::touchPressed(){
    cout << "Button active in Screen Saver!" << endl;
    ofNotifyEvent(onClick, targetScene);
}

ofEvent <int> vhpScreenSaver::onClick;
