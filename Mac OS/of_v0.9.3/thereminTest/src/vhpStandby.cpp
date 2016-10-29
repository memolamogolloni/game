#include "vhpStandby.h"

// Constructor -------------------------------------------------

vhpStandby::vhpStandby():loaded(false),loading(false){

}

vhpStandby::~vhpStandby(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------
void vhpStandby::setup(int _currentScene, int _targetScene){
    
    // Inicializar las variables
    currentScene = _currentScene;       // SCREENSAVER
    targetScene = _targetScene;         // PLAYERMENU
    
    // FBO alocation and cleaning
    width = 1920;
    height = 1080;
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
    // Añadir las fuentes
    TTF.load("fonts/titilliumweblight.ttf", 22);
    TTFB.load("fonts/titilliumweblight.ttf", 70);
    
    // elementos gráficos
    
    // Añadir las imágenes Sueltas
    loadingSilge.push_back(&clock);
    filesSingle.push_back("sb-clock");
    loadingSilge.push_back(&cancel);
    filesSingle.push_back("sb-cancel");
    loadingSilge.push_back(&bg);
    filesSingle.push_back("sb-bg");
    loadingSilge.push_back(&balls);
    filesSingle.push_back("sb-balls");
    loadingSilge.push_back(&bases);
    filesSingle.push_back("sb-bases");
    loadingSilge.push_back(&shadow);
    filesSingle.push_back("sb-shadow");
    loadingSilge.push_back(&back);
    filesSingle.push_back("sb-volver");
    
    currentLoad = &vhpStandby::loadSingle;

}
void vhpStandby::getText(string _file) {
    ofBuffer buffer = ofBufferFromFile(_file);
    for (auto line : buffer.getLines()){
        cout << line << endl;
        lines.push_back(line);
    }
}

// Precarga de todos los elementos -----------------------------
void vhpStandby::load(){
    (*this.*currentLoad)();
}
void vhpStandby::loadSingle(){
    if (!loaded) {
        int actual = filesSingle.size() -1;
        if (actual>=0) {
            if(loadingSilge[actual]->isAllocated()) {
                loadingSilge.pop_back();
                filesSingle.pop_back();
                loading = false;
            } else {
                if (!loading) {
                    loadingSilge[actual]->load("images/"+ filesSingle[actual] +".png");
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
void vhpStandby::setWaiting(){
    lines.clear();
    getText("txt/sb-waiting.txt");
    alpha = 0;
    alpha_increment = 5;
    currentUpdate = &vhpStandby::fadeInWaiting;
    
    count = 0;
    countDown = 30;
}
void vhpStandby::setAcepted(){
    lines.clear();
    getText("txt/sb-acepted.txt");
    alpha = 0;
    alpha_increment = 5;
    currentUpdate = &vhpStandby::fadeInAcepted;
    state = WAITING;
}
void vhpStandby::setCancel(){
    lines.clear();
    getText("txt/sb-cancel.txt");
    alpha = 0;
    alpha_increment = 5;
    currentUpdate = &vhpStandby::fadeInCancel;
    state = CANCELED;
}
void vhpStandby::start(){
    cout << "Standby start();" << endl;
}

void vhpStandby::stop(){
}

// Dibujado y actualización variables --------------------------
void vhpStandby::update(){
    (*this.*currentUpdate)();
}
void vhpStandby::updateElements(){
    count++;
    if (count%20==0){
        countDown--;
        if (countDown<=0) {
            countDown=0;
        }
    }
}

// -------------------------------------------------------------
void vhpStandby::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Dibujado de elementos gráficos ------------------------------
void vhpStandby::drawWaiting(){
    ofSetColor(255, 255, 255);
    bg.draw(0,0);
    bases.drawSubsection(0, 0, 960, 1080, 0, 0);
    balls.drawSubsection(0, 0, 960, 1080, 0, 0);
    shadow.drawSubsection(960, 0, 960, 1080, 960, 0);
    ofSetColor(255, 255 ,255 ,alpha);
    clock.draw(0,0);
    TTFB.drawString(lines[0], 535, 490);
    TTFB.drawString(lines[1], 600, 590);
    ofSetColor(24, 194, 201,alpha);
    TTFB.drawString(ofToString(countDown), 930, 395);
    ofSetColor(255, 255, 255);
}
void vhpStandby::drawCancel(){
    ofSetColor(255, 255, 255);
    bg.draw(0,0);
    bases.drawSubsection(0, 0, 960, 1080, 0, 0);
    balls.drawSubsection(0, 0, 960, 1080, 0, 0);
    shadow.drawSubsection(960, 0, 960, 1080, 960, 0);
    ofSetColor(255, 255 ,255 ,alpha);
    cancel.draw(0,0);
    TTFB.drawString(lines[0], 610, 510);
    TTFB.drawString(lines[1], 750, 600);
    back.draw(825,691);
    ofSetColor(255, 255, 255);
}


// Reproducir o detener la escena modificando currentUpdate ----
void vhpStandby::play(){
    setWaiting();
    currentUpdate = &vhpStandby::fadeInWaiting;
}

// Procesado y actualización -----------------------------------
void vhpStandby::fadeInWaiting(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawWaiting();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) alpha = 255;
    if (countDown==0) {
        currentUpdate = &vhpStandby::fadeOutWaiting;
    }
}
void vhpStandby::fadeOutWaiting(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawWaiting();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0;
        setCancel();
        /*
        setNext();
         */
    }
}
void vhpStandby::fadeInCancel(){
    count++;
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawCancel();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) alpha = 255;
}

// Eventos ------------------------------------------------------
void vhpStandby::touchPressed(){
    cout << "Button active in Standby!" << endl;
    if (state == CANCELED) {
        ofNotifyEvent(onCancel, targetScene);
    }
}

ofEvent <int> vhpStandby::onCancel;
