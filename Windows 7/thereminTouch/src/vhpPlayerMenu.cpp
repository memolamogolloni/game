#include "vhpPlayerMenu.h"

// Constructor -------------------------------------------------
vhpPlayerMenu::vhpPlayerMenu():state(MENU),selected(0),loaded(false),loading(false){

}
vhpPlayerMenu::~vhpPlayerMenu(){
    stop();
}

// Inicializar variables y cargar los archivos -----------------
void vhpPlayerMenu::setup(int _currentScene, int _targetScene1, int _targetScene2){
    
    // Inicializar las variables
    currentScene = _currentScene;       // PLAYERMENU
    targetScene[0] = _targetScene1;     // LEVELMENU
    targetScene[1] = _targetScene2;     // STANDBY
    cout << "LEVELMENU " << targetScene[0] << endl;
    cout << "STANDBY " << targetScene[1] << endl;
    
    // FBO alocation and cleaning
    width = 1920;
    height = 1080;
    fbo.allocate(width, height, GL_RGBA);
    transparent.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    transparent.begin();
    ofClear(255,255,255, 0);
    transparent.end();
    
    // elementos gráficos
    
    // Añadir las imágenes Sueltas
    loadingSilge.push_back(&glow);
    filesSingle.push_back("pm-glow");
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
    
    currentLoad = &vhpPlayerMenu::loadSingle;
    
}
void vhpPlayerMenu::setupResources(ofImage* _bg, ofImage* _colorBarR, ofImage* _colorBarA, vhpPetamuti* _petamuti) {
    colorBar[0] = _colorBarR;
    colorBar[1] = _colorBarA;
    petamuti = _petamuti;
    bg = _bg;
    petamuti->setAimationFull();
}
void vhpPlayerMenu::setupFonts(ofxTrueTypeFontUC* _TTF, ofxTrueTypeFontUC* _TTFB, ofxTrueTypeFontUC* _TTFM){
    TTF = _TTF;
    TTFB = _TTFB;
    TTFM = _TTFM;
}
void vhpPlayerMenu::getText(string _file) {
    ofBuffer buffer = ofBufferFromFile(_file);
    for (int i = 0; i < buffer.size(); i++) {
        lines.push_back(vhpLine(buffer.getNextLine()));
        cout << lines[i].full << endl;
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
void vhpPlayerMenu::init(){
    lines.clear();
    getText("txt/pm-intro.txt");
    alpha = 0.0;
    alpha_increment = 10.0;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    state = MENU;
    target = MENU;
}
void vhpPlayerMenu::setOne(){
    lines.clear();
    getText("txt/pm-1vsai.txt");
    alpha = 0.0;
    alpha_increment = 10.0;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    drawButtons(1);
    currentUpdate = &vhpPlayerMenu::fadeInOne;
    state = ONEPLAYER;
    target = MENU;
    
}
void vhpPlayerMenu::setTwo(){
    lines.clear();
    getText("txt/pm-1vs1.txt");
    alpha = 0.0;
    alpha_increment = 10.0;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    drawButtons(2);
    currentUpdate = &vhpPlayerMenu::fadeInTwo;
    state = TWOPLAYERS;
    target = MENU;
}
void vhpPlayerMenu::setFour(){
    lines.clear();
    getText("txt/pm-2vs2.txt");
    
    cout << "txt/pm-2vs2.txt" << endl;
    alpha = 0.0;
    alpha_increment = 10.0;
    for (int i = 0; i < lines.size(); i++) {
        lines[i].init();
    }
    count = 0;
    drawButtons(4);
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
    // cout << count << endl;
    updateTextLine();
}
void vhpPlayerMenu::updateTextLine(){
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
void vhpPlayerMenu::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Dibujado de elementos gráficos ------------------------------
void vhpPlayerMenu::drawTextLine(int _x, int _y, int _alpha){
    ofSetColor(255, 255, 255, _alpha);
    for (int i = 0; i < lines.size(); i++) {
        TTF->drawString(lines[i].visible, _x, _y +(40*i));
    }
}
void vhpPlayerMenu::drawChooseButton(int _x, int _y){
    ofRectangle box;
    box = TTFM->getStringBoundingBox("EE", 0, 0);
    int margin = box.width;
    ofSetRectMode(OF_RECTMODE_CORNER);
    box = TTFM->getStringBoundingBox("Elegir", 0, 0);
    colorBar[0]->draw(_x - margin, _y, box.width + margin*2 + 12, TTFM->getLineHeight() + 12);
    TTFM->drawString("Elegir", _x, _y + TTFM->getLineHeight()*3.0/4.0);
}
void vhpPlayerMenu::drawButtons(int _n) {
    ofPushStyle();
    ofEnableAlphaBlending();
    transparent.begin();
    ofClear(255,255,255, 0);
    switch (_n) {
        case 1:
            drawChooseButton(318, 866);
            TTFB->drawString("1 vs AI", 285, 566);
            break;
        case 2:
            drawChooseButton(775, 866);
            TTFB->drawString("1 vs 1", 750, 566);
            break;
        case 4:
            drawChooseButton(1320, 866);
            TTFB->drawString("2 vs 2", 1230, 566);
            break;
        default:
            break;
    }
    transparent.end();
    ofDisableAlphaBlending();
    ofPopStyle();
}
void vhpPlayerMenu::drawMenu(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    bases.draw(0,0);
    balls.draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    petamuti->draw(76, 51);
}
void vhpPlayerMenu::drawOne(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    one.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    balls.draw(0,0);
    petamuti->draw(76, 51);
    ofSetColor(255,255,255,alpha);
    transparent.draw(0,0);
}
void vhpPlayerMenu::drawTwo(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    two.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    balls.draw(0,0);
    petamuti->draw(76, 51);
    ofSetColor(255,255,255,alpha);
    transparent.draw(0,0);
}
void vhpPlayerMenu::drawFour(){
    ofSetColor(255, 255, 255);
    bg->draw(0,0);
    ofSetColor(255,255,255,alpha);
    glow.draw(0,0,width*0.8,height);
    four.draw(0,0);
    drawTextLine(425, 200, alpha);
    ofSetColor(255, 255, 255);
    bases.draw(0,0);
    balls.draw(0,0);
    petamuti->draw(76, 51);
    ofSetColor(255,255,255,alpha);
    transparent.draw(0,0);
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
        alpha_increment = 10.0;
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
        alpha_increment = 10.0;
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
        alpha_increment = 10.0;
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
        alpha_increment = 10.0;
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
                            cout << "ONEPLAYER confirmed! target: " << targetScene[0] << endl;
                            int val = 3; // LEVELMENU & targetScene[0] ?
                            ofNotifyEvent(playersNumber, val);
                        }
                        break;
                    case TWOPLAYERS:
                        target = ONEPLAYER;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case FOURPLAYERS:
                        target = ONEPLAYER;
                        alpha_increment = 20.0;
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
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case TWOPLAYERS:
                        if ((_y>=840)&&(_y<=960)) {
                            selected = 2;
                            cout << "TWOPLAYERS confirmed! target: " << targetScene[0] << endl;
                            int val = 3; // LEVELMENU & targetScene[0] ?
                            ofNotifyEvent(playersNumber, val);
                        }
                        break;
                    case FOURPLAYERS:
                        target = TWOPLAYERS;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                }
            } else {
                cout << "4 xogadores" << endl;
                cout << "state: " << state << endl;
                /*
                switch (state) {
                    case MENU:
                        cout << "setFour();" << endl;
                        setFour();
                        break;
                    case ONEPLAYER:
                        target = FOURPLAYERS;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case TWOPLAYERS:
                        target = FOURPLAYERS;
                        alpha_increment = 20.0;
                        count=500;
                        break;
                    case FOURPLAYERS:
                        if ((_y>=840)&&(_y<=960)) {
                            selected = 4;
                            cout << "FOURPLAYERS confirmed! target: " << targetScene[1] << endl;
                            int val = 2; // STANDBY & targetScene[1] ?
                            ofNotifyEvent(playersNumber, val);
                        }
                        break;
                }
                 */
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