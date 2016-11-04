#include "vhpLevelMenu.h"

// Constructor -------------------------------------------------

vhpLevelMenu::vhpLevelMenu():state(MENUNIVEL),selected(0),loaded(false),loading(false){

}

vhpLevelMenu::~vhpLevelMenu(){
}

// Inicializar variables y cargar los archivos -----------------
void vhpLevelMenu::setup(int _currentScene, int _targetScene){
    
    // Inicializar las variables
    currentScene = _currentScene;   // PLAYERMENU
    targetScene = _targetScene;     // GAME
    
    // FBO alocation and cleaning
    width = 1920;
    height = 1080;
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
    // elementos gr‡ficos
    
    // A–adir las im‡genes Sueltas
    loadingSilge.push_back(&glow);
    filesSingle.push_back("lm-glow");
    loadingSilge.push_back(&icons);
    filesSingle.push_back("lm-icons");
    loadingSilge.push_back(&bases);
    filesSingle.push_back("lm-bases");
    loadingSilge.push_back(&destreza);
    filesSingle.push_back("lm-destreza");
    loadingSilge.push_back(&espiritualidad);
    filesSingle.push_back("lm-espiritualidad");
    loadingSilge.push_back(&oratoria);
    filesSingle.push_back("lm-oratoria");
    
    currentLoad = &vhpLevelMenu::loadSingle;
    
}
void vhpLevelMenu::setupResources(ofImage* _bg, ofImage* _keko) {
    bg = _bg;
    keko = _keko;
}
void vhpLevelMenu::setupFonts(ofTrueTypeFont* _TTF, ofTrueTypeFont* _TTFB){
    TTF = _TTF;
    TTFB = _TTFB;
}
void vhpLevelMenu::getText(string _file) {
    ofBuffer buffer = ofBufferFromFile(_file);
    for (auto line : buffer.getLines()){
        cout << line << endl;
        lines.push_back(vhpLine(line));
    }
}

// Precarga de todos los elementos -----------------------------
void vhpLevelMenu::load(){
    (*this.*currentLoad)();
}
void vhpLevelMenu::loadSingle(){
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
            cout << "loading single images in Level Menu finished!" << endl;
            loaded = true;
        }
    }
}

// Comenzar e interrumpir los hilos y listeners de la escena ---
void vhpLevelMenu::init(){
    lines.clear();
    getText("txt/lm-intro.txt");
    alpha = 0.0;
    alpha_increment = 10.0;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    target = MENUNIVEL;
}
void vhpLevelMenu::setDestreza(){
    count = 0;
    currentUpdate = &vhpLevelMenu::fadeInDestreza;
    state = DESTREZA;
    target = MENUNIVEL;
}
void vhpLevelMenu::setEspiritualidad(){
    count = 0;
    currentUpdate = &vhpLevelMenu::fadeInEspiritualidad;
    state = ESPIRITUALIDAD;
    target = MENUNIVEL;
}
void vhpLevelMenu::setOratoria(){
    count = 0;
    currentUpdate = &vhpLevelMenu::fadeInOratoria;
    state = ORATORIA;
    target = MENUNIVEL;
}
void vhpLevelMenu::setNext(){
    switch (target) {
        case MENUNIVEL:
            currentUpdate = &vhpLevelMenu::emptyMenu;
            state = MENUNIVEL;
            target = MENUNIVEL;
            break;
        case DESTREZA:
            setDestreza();
            break;
        case ESPIRITUALIDAD:
            setEspiritualidad();
            break;
        case ORATORIA:
            setOratoria();
            break;
    }
}
void vhpLevelMenu::start(){
}
void vhpLevelMenu::stop(){
}

// Dibujado y actualizaci—n variables --------------------------
void vhpLevelMenu::update(){
    (*this.*currentUpdate)();
}
void vhpLevelMenu::updateElements(){
    count++;
    // cout << count << endl;
    updateTextLine();
}
void vhpLevelMenu::updateTextLine(){
    if (count%3==0){
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].isNotLast()) {
                lines[i].add();
                break;
            }
        }
    }
}

//--------------------------------------------------------------
void vhpLevelMenu::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Dibujado de elementos gr‡ficos ------------------------------
void vhpLevelMenu::drawTextLine(int _x, int _y, int _alpha){
    ofSetColor(255, 255, 255, _alpha);
    for (int i = 0; i < lines.size(); i++) {
        TTF->drawString(lines[i].visible, _x, _y +(40*i));
    }
}
void vhpLevelMenu::drawMenu(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    bases.draw(0,0);
    icons.draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    keko->draw(0,0);
}
void vhpLevelMenu::drawDestreza(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    destreza.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    icons.draw(0,0);
    keko->draw(0,0);
}
void vhpLevelMenu::drawEspiritualidad(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    espiritualidad.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    icons.draw(0,0);
    keko->draw(0,0);
}
void vhpLevelMenu::drawOratoria(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    oratoria.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    icons.draw(0,0);
    keko->draw(0,0);
}

// Reproducir o detener la escena modificando currentUpdate ----

void vhpLevelMenu::play(){
    currentUpdate = &vhpLevelMenu::fadeInText;
    init();
}

// Procesado y actualizaci—n -----------------------------------
void vhpLevelMenu::emptyMenu(){
    count++;
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawMenu();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
}
void vhpLevelMenu::fadeInText(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawMenu();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        if (count>=500) currentUpdate = &vhpLevelMenu::fadeOutText;
    }
}
void vhpLevelMenu::fadeOutText(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawMenu();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0.0;
        alpha_increment = 10.0;
        setNext();
    }
}
void vhpLevelMenu::fadeInDestreza(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawDestreza();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        if (count>=500) currentUpdate = &vhpLevelMenu::fadeOutDestreza;
    }
}
void vhpLevelMenu::fadeOutDestreza(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawDestreza();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0.0;
        alpha_increment = 10.0;
        setNext();
    }
}
void vhpLevelMenu::fadeInEspiritualidad(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawEspiritualidad();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        if (count>=500) currentUpdate = &vhpLevelMenu::fadeOutEspiritualidad;
    }
}
void vhpLevelMenu::fadeOutEspiritualidad(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawEspiritualidad();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0.0;
        alpha_increment = 10.0;
        setNext();
    }
}
void vhpLevelMenu::fadeInOratoria(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawOratoria();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        if (count>=500) currentUpdate = &vhpLevelMenu::fadeOutOratoria;
    }
}
void vhpLevelMenu::fadeOutOratoria(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawOratoria();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0.0;
        setNext();
    }
}

// Utilidades ---------------------------------------------------
void vhpLevelMenu::alert(int _e){
    cout << " Event detected " << _e << endl;
}

// Eventos ------------------------------------------------------

void vhpLevelMenu::touchPressed(float _x, float _y){
    cout << "Menu active!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    // min x: 520, min y: 182 408 538 630 160
    cout << "mouse x: " << _x << " mouse y: " << _y << endl;
    
    if ((_y>=540)&&(_y<=740)&&(_x>=740)&&(_x<=1180)) {
        switch (state) {
            case MENUNIVEL:
                break;
            case DESTREZA:
                selected = DESTREZA;
                ofNotifyEvent(levelSelection, selected);
                cout << "DESTREZA confirmed!" << endl;
                break;
            case ESPIRITUALIDAD:
                selected = ESPIRITUALIDAD;
                ofNotifyEvent(levelSelection, selected);
                cout << "ESPIRITUALIDAD confirmed!" << endl;
                break;
            case ORATORIA:
                selected = ORATORIA;
                ofNotifyEvent(levelSelection, selected);
                cout << "ORATORIA confirmed!" << endl;
                break;
        }
    } else if (_y>=520) {
        if ((_x>=182)&&(_x<=1760)) {
            if (_x<=592) {
                cout << "DESTREZA" << endl;
                switch (state) {
                    case MENUNIVEL:
                        setDestreza();
                        break;
                    case DESTREZA:
                        break;
                    case ESPIRITUALIDAD:
                        target = DESTREZA;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case ORATORIA:
                        target = DESTREZA;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                }
            } else if (_x<=1130) {
                cout << "ESPIRITUALIDAD" << endl;
                switch (state) {
                    case MENUNIVEL:
                        setEspiritualidad();
                        break;
                    case DESTREZA:
                        target = ESPIRITUALIDAD;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case ESPIRITUALIDAD:
                        break;
                    case ORATORIA:
                        target = ESPIRITUALIDAD;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                }
            } else {
                cout << "ORATORIA" << endl;
                switch (state) {
                    case MENUNIVEL:
                        setOratoria();
                        break;
                    case DESTREZA:
                        target = ORATORIA;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case ESPIRITUALIDAD:
                        target = ORATORIA;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case ORATORIA:
                        break;
                }
            }
        } else {
            cout << "fora" << endl;
            target = MENUNIVEL;
            count=500;
        }
    } else {
        if (state != MENUNIVEL) {
            cout << "fora" << endl;
            target = MENUNIVEL;
            count=500;
        }
    }
}

ofEvent <int> vhpLevelMenu::levelSelection;
