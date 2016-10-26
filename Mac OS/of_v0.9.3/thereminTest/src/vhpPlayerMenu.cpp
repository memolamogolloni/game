#include "vhpPlayerMenu.h"

// Constructor -------------------------------------------------
vhpPlayerMenu::vhpPlayerMenu():state(MENU),selected(0){

}
vhpPlayerMenu::~vhpPlayerMenu(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------
void vhpPlayerMenu::setup(int _currentScene, int _targetScene){
    
    // Inicializar las variables
    currentScene = _currentScene;   // PLAYERMENU
    targetScene = _targetScene;     // STANDBY
    
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
    loadingSilge.push_back(&glow);
    filesSingle.push_back("pm-glow");
    loadingSilge.push_back(&keko);
    filesSingle.push_back("pm-keko");
    loadingSilge.push_back(&bg);
    filesSingle.push_back("pm-bg");
    loadingSilge.push_back(&balls);
    filesSingle.push_back("pm-balls");
    loadingSilge.push_back(&bases);
    filesSingle.push_back("pm-bases");
    loadingSilge.push_back(&one);
    filesSingle.push_back("pm-one");
    loadingSilge.push_back(&two);
    filesSingle.push_back("pm-two");
    loadingSilge.push_back(&four);
    filesSingle.push_back("pm-four");
    loadingSilge.push_back(&elegir);
    filesSingle.push_back("pm-elegir");
    
    currentLoad = &vhpPlayerMenu::loadSingle;
    
}
void vhpPlayerMenu::getText(string _file) {
    ofBuffer buffer = ofBufferFromFile(_file);
    for (auto line : buffer.getLines()){
        cout << line << endl;
        lines.push_back(vhpLine(line));
    }
}

// Precarga de todos los elementos -----------------------------
void vhpPlayerMenu::load(){
    (*this.*currentLoad)();
}
void vhpPlayerMenu::loadSingle(){
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
void vhpPlayerMenu::init(){
    lines.clear();
    getText("txt/pm-intro.txt");
    alpha = 0;
    alpha_increment = 5;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    target = MENU;
}
void vhpPlayerMenu::setOne(){
    lines.clear();
    getText("txt/pm-1vsai.txt");
    alpha = 0;
    alpha_increment = 5;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    currentUpdate = &vhpPlayerMenu::fadeInOne;
    state = ONEPLAYER;
    target = MENU;
}
void vhpPlayerMenu::setTwo(){
    lines.clear();
    getText("txt/pm-1vs1.txt");
    alpha = 0;
    alpha_increment = 5;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    currentUpdate = &vhpPlayerMenu::fadeInTwo;
    state = TWOPLAYERS;
    target = MENU;
}
void vhpPlayerMenu::setFour(){
    lines.clear();
    getText("txt/pm-2vs2.txt");
    alpha = 0;
    alpha_increment = 5;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    currentUpdate = &vhpPlayerMenu::fadeInFour;
    state = FOURPLAYERS;
    target = MENU;
}
void vhpPlayerMenu::setNext(){
    switch (target) {
        case MENU:
            currentUpdate = &vhpPlayerMenu::emptyMenu;
            state = MENU;
            target = MENU;
            break;
        case ONEPLAYER:
            setOne();
            break;
        case TWOPLAYERS:
            setTwo();
            break;
        case FOURPLAYERS:
            setFour();
            break;
    }
}
void vhpPlayerMenu::start(){
}
void vhpPlayerMenu::stop(){
}

// Dibujado y actualización variables --------------------------
void vhpPlayerMenu::update(){
    (*this.*currentUpdate)();
}
void vhpPlayerMenu::updateElements(){
    count++;
    cout << count << endl;
    updateTextLine();
}
void vhpPlayerMenu::updateTextLine(){
    if (count%6==0){
        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].isNotLast()) {
                lines[i].add();
                break;
            }
        }
    }
}

//--------------------------------------------------------------
void vhpPlayerMenu::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Dibujado de elementos gráficos ------------------------------
void vhpPlayerMenu::drawTextLine(int _x, int _y, int _alpha){
    ofSetColor(255, 255, 255, _alpha);
    for (int i = 0; i < lines.size(); i++) {
        TTF.drawString(lines[i].visible, _x, _y +(40*i));
    }
}
void vhpPlayerMenu::drawMenu(){
    ofSetColor(255, 255, 255);
    bg.draw(0,0);
    bases.draw(0,0);
    balls.draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    keko.draw(0,0);
}
void vhpPlayerMenu::drawOne(){
    ofSetColor(255, 255, 255);
    bg.draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    one.draw(0,0);
    drawTextLine(425, 200, alpha);
    TTFB.drawString("1 vs AI", 255, 566);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    balls.draw(0,0);
    keko.draw(0,0);
    ofSetColor(255,255,255,alpha);
    elegir.draw(249, 866);
}
void vhpPlayerMenu::drawTwo(){
    ofSetColor(255, 255, 255);
    bg.draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    two.draw(0,0);
    drawTextLine(425, 200, alpha);
    TTFB.drawString("1 vs 1", 740, 566);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    balls.draw(0,0);
    keko.draw(0,0);
    ofSetColor(255,255,255,alpha);
    elegir.draw(716, 866);
}
void vhpPlayerMenu::drawFour(){
    ofSetColor(255, 255, 255);
    bg.draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    four.draw(0,0);
    drawTextLine(425, 200, alpha);
    TTFB.drawString("2 vs 2", 1230, 566);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    balls.draw(0,0);
    keko.draw(0,0);
    ofSetColor(255,255,255,alpha);
    elegir.draw(1320, 866);
}

// Reproducir o detener la escena modificando currentUpdate ----
void vhpPlayerMenu::play(){
    currentUpdate = &vhpPlayerMenu::fadeInText;
    init();
}

// Procesado y actualización -----------------------------------
void vhpPlayerMenu::emptyMenu(){
    count++;
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawMenu();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
}
void vhpPlayerMenu::fadeInText(){
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
        if (count>=500) currentUpdate = &vhpPlayerMenu::fadeOutText;
    }
}
void vhpPlayerMenu::fadeOutText(){
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
        alpha = 0;
        alpha_increment = 5;
        setNext();
    }
}
void vhpPlayerMenu::fadeInOne(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawOne();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        if (count>=500) currentUpdate = &vhpPlayerMenu::fadeOutOne;
    }
}
void vhpPlayerMenu::fadeOutOne(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawOne();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0;
        alpha_increment = 5;
        setNext();
    }
}
void vhpPlayerMenu::fadeInTwo(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawTwo();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        if (count>=500) currentUpdate = &vhpPlayerMenu::fadeOutTwo;
    }
}
void vhpPlayerMenu::fadeOutTwo(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawTwo();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0;
        alpha_increment = 5;
        setNext();
    }
}
void vhpPlayerMenu::fadeInFour(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawFour();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        if (count>=500) currentUpdate = &vhpPlayerMenu::fadeOutFour;
    }
}
void vhpPlayerMenu::fadeOutFour(){
    updateElements();
    
    ofPushStyle();
    ofEnableAlphaBlending();
    fbo.begin();
    drawFour();
    fbo.end();
    ofDisableAlphaBlending();
    ofPopStyle();
    
    alpha -= alpha_increment;
    if (alpha<=0) {
        alpha = 0;
        alpha_increment = 5;
        setNext();
    }
}

// Utilidades ---------------------------------------------------
void vhpPlayerMenu::alert(int _e){
    cout << " Event detected " << _e << endl;
}

// Eventos ------------------------------------------------------

void vhpPlayerMenu::touchPressed(float _x, float _y){
    cout << "Menu active!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    // min x: 520, min y: 182 408 538 630 160
    cout << "mouse x: " << _x << " mouse y: " << _y << endl;
    if (_y>=520) {
        if ((_x>=182)&&(_x<=1760)) {
            if (_x<=592) {
                cout << "1 xogador" << endl;
                switch (state) {
                    case MENU:
                        setOne();
                        break;
                    case ONEPLAYER:
                        if ((_y>=840)&&(_y<=960)) {
                            selected = 1;
                            ofNotifyEvent(playersNumber, targetScene);
                            cout << "ONEPLAYER confirmed!" << endl;
                        }
                        break;
                    case TWOPLAYERS:
                        target = ONEPLAYER;
                        alpha_increment = 10;
                        count=500;
                        break;
                    case FOURPLAYERS:
                        target = ONEPLAYER;
                        alpha_increment = 10;
                        count=500;
                        break;
                }
            } else if (_x<=1130) {
                cout << "2 xogadores" << endl;
                switch (state) {
                    case MENU:
                        setTwo();
                        break;
                    case ONEPLAYER:
                        target = TWOPLAYERS;
                        alpha_increment = 10;
                        count=500;
                        break;
                    case TWOPLAYERS:
                        if ((_y>=840)&&(_y<=960)) {
                            selected = 2;
                            ofNotifyEvent(playersNumber, targetScene);
                            cout << "TWOPLAYERS confirmed!" << endl;
                        }
                        break;
                    case FOURPLAYERS:
                        target = TWOPLAYERS;
                        alpha_increment = 10;
                        count=500;
                        break;
                }
            } else {
                cout << "4 xogadores" << endl;
                switch (state) {
                    case MENU:
                        setFour();
                        break;
                    case ONEPLAYER:
                        target = FOURPLAYERS;
                        alpha_increment = 10;
                        count=500;
                        break;
                    case TWOPLAYERS:
                        target = FOURPLAYERS;
                        alpha_increment = 10;
                        count=500;
                        break;
                    case FOURPLAYERS:
                        if ((_y>=840)&&(_y<=960)) {
                            selected = 4;
                            ofNotifyEvent(playersNumber, targetScene);
                            cout << "FOURPLAYERS confirmed!" << endl;
                        }
                        break;
                }
            }
        } else {
            cout << "fora" << endl;
            target = MENU;
            count=500;
        }
    } else {
        if (state != MENU) {
            cout << "fora" << endl;
            target = MENU;
            count=500;
        }
    }
}

ofEvent <int> vhpPlayerMenu::playersNumber;
