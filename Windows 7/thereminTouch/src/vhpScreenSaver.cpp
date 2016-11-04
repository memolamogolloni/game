#include "vhpScreenSaver.h"

// Constructor -------------------------------------------------

vhpScreenSaver::vhpScreenSaver():loaded(false), loading(false){

}

vhpScreenSaver::~vhpScreenSaver(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------
void vhpScreenSaver::setup(int _currentScene, int _targetScene){
    
    // Inicializar las variables
    currentScene = _currentScene;   // SCREENSAVER
    targetScene = _targetScene;     // PLAYERMENU
    alpha = 0.0;
    alpha_increment = 4.0;
    salpha = 0.0;
    salpha_increment = 2.0;
    finishLoading = false;
    
    // FBO alocation and cleaning
    width = 1920;
    height = 1080;
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
    // Añadir las imágenes Sueltas
    loadingSilge.push_back(&bg);
    filesSingle.push_back("ss-bg");
    loadingSilge.push_back(&theremin);
    filesSingle.push_back("ss-theremin");
    loadingSilge.push_back(&loadingtxt);
    filesSingle.push_back("ss-loading");
    
    currentLoad = &vhpScreenSaver::loadSingle;
    
}                           
                           
// Precarga de todos los elementos -----------------------------
void vhpScreenSaver::load(){
    (*this.*currentLoad)();
}
void vhpScreenSaver::loadSingle(){
    if (!loaded) {
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
            cout << "loading single images in Player Menu finished!" << endl;
            loaded = true;
        }
    }
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
    //video.play();
}

void vhpScreenSaver::pause(){
    currentUpdate = &vhpScreenSaver::pause;
    //video.stop();
}

// Procesado y actualización -----------------------------------

void vhpScreenSaver::playScreenSaver(){
    // Reproduce el video y lo dibuja en el FBO
    //video.update();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    fbo.begin();
    ofClear(0,0,0,0);
    //video.draw(0, 0);
    ofSetColor(255, 255, 255, salpha);
    bg.draw(0, 0);
    theremin.draw(0, 0);
    ofSetColor(255, 255, 255, (alpha/255)*salpha);
    loadingtxt.draw(0, 0);
    fbo.end();
    ofPopStyle();
    salpha += salpha_increment;
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        alpha_increment = -1 * alpha_increment;
    } else if (alpha<=0) {
        alpha = 0;
        if (!finishLoading) alpha_increment = -1 * alpha_increment;
    }
}

void vhpScreenSaver::pauseScreenSaver(){
    // Al no hacer nada mantiene el FBO con el último fotograma reproducido
}

void vhpScreenSaver::loopScreenSaver(float _pos){
    // Envia la cabeza lectora del video a un punto determinado
    //video.setPosition(_pos);
}

// Eventos ------------------------------------------------------
void vhpScreenSaver::touchPressed(){
    cout << "Button active in Screen Saver!" << endl;
    ofNotifyEvent(onClick, targetScene);
}

ofEvent <int> vhpScreenSaver::onClick;
