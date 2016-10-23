#include "vhpWindow.h"

//--------------------------------------------------------------
vhpWindow::vhpWindow(){

}

//--------------------------------------------------------------
void vhpWindow::setup(){
    fadeDuration = 200;
    visibleDuration = 600;
    hiddenDuration = 200;
    order = 0;
}
void vhpWindow::setWindows(ofImage* _purple, ofImage* _yellow, ofImage* _blue, ofImage* _green){
    purple = _purple;
    yellow = _yellow;
    blue = _blue;
    green = _green;
}
void vhpWindow::setActualWindow() {
    switch (order) {
        case 1:
            window = purple;
            break;
        case 2:
            window = yellow;
            break;
        case 3:
            window = blue;
            break;
        case 4:
            window = green;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void vhpWindow::update(){
    (*this.*currentUpdate)();
}
void vhpWindow::updateFadeIn(){
    if (alpha==255) setVisible();
    setAlpha();
}
void vhpWindow::updateVisible(){
    if (ofGetElapsedTimeMillis()>=(start + visibleDuration)) setFadeOut();
}
void vhpWindow::updateFadeOut(){
    if (alpha==0) setHidden();
    setAlpha();
}
void vhpWindow::updateHidden(){
    if ((order<4)&&(ofGetElapsedTimeMillis()>=(start + hiddenDuration))) setFadeIn();
}

//--------------------------------------------------------------

void vhpWindow::draw(){
    (*this.*currentDraw)();
}
void vhpWindow::drawFadeIn(){
    ofSetColor(255,255,255,alpha);
    window->draw(0, 0);
}
void vhpWindow::drawVisible(){
    ofSetColor(255,255,255,255);
    window->draw(0, 0);
}
void vhpWindow::drawFadeOut(){
    ofSetColor(255,255,255,alpha);
    window->draw(0, 0);
}
void vhpWindow::drawHidden(){
    
}

//--------------------------------------------------------------
void vhpWindow::setFadeIn(){
    order ++;
    setActualWindow();
    currentUpdate = &vhpWindow::updateFadeIn;
    currentDraw = &vhpWindow::drawFadeIn;
    setStart();
    state = fadeIn;
}
void vhpWindow::setVisible(){
    currentUpdate = &vhpWindow::updateVisible;
    currentDraw = &vhpWindow::drawVisible;
    setStart();
    state = visible;
}
void vhpWindow::setFadeOut(){
    currentUpdate = &vhpWindow::updateFadeOut;
    currentDraw = &vhpWindow::drawFadeOut;
    setStart();
    state = fadeOut;
}
void vhpWindow::setHidden(){
    currentUpdate = &vhpWindow::updateHidden;
    currentDraw = &vhpWindow::drawHidden;
    setStart();
    state = hidden;
}


//--------------------------------------------------------------
void vhpWindow::setStart(){
    start = ofGetElapsedTimeMillis();
}
void vhpWindow::setAlpha(){
    alpha = ofMap(ofGetElapsedTimeMillis(), start, start + fadeDuration, 0, 255, true);
}