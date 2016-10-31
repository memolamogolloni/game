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
    
    wHeight = 223;
    wY = 335;
    wWidth[0][0] = 172;
    wWidth[0][1] = 146;
    wWidth[0][2] = 138;
    wWidth[0][3] = 131;
    wWidth[0][4] = 125;
    wWidth[0][5] = 120;
    wWidth[0][6] = 128;
    wWidth[1][6] = 172;
    wWidth[1][5] = 146;
    wWidth[1][4] = 138;
    wWidth[1][3] = 131;
    wWidth[1][2] = 125;
    wWidth[1][1] = 120;
    wWidth[1][0] = 128;
    
    wX[0][0] = 0;
    for (int i = 1; i < 7; i++) {
        wX[0][i] = wX[0][i-1] + wWidth[0][i-1];
    }
    wX[1][0] = wX[0][6] + wWidth[0][6];
    for (int i = 1; i < 7; i++) {
        wX[1][i] = wX[1][i-1] + wWidth[1][i-1];
    }

    n = 0;
}
void vhpWindow::setWindows(ofImage* _purple, int _nP, ofImage* _yellow, int _nY, ofImage* _blue, int _nB, ofImage* _green, int _nG){
    nW[0] = _nP;
    nW[1] = _nY;
    nW[2] = _nB;
    nW[3] = _nG;
    purple = _purple;
    yellow = _yellow;
    blue = _blue;
    green = _green;
}
void vhpWindow::setActualWindow() {
    switch (order) {
        case 1:
            n = nW[0];
            window = purple;
            break;
        case 2:
            n = nW[1];
            window = yellow;
            break;
        case 3:
            n = nW[2];
            window = blue;
            break;
        case 4:
            n = nW[3];
            window = green;
            break;
        default:
            break;
    }
}
void vhpWindow::setOneWindow(int _order, int _n, int _player) {
    player = _player;
    n = _n;
    cout << "order: " << _order << " n: " << _n << " player: " << _player << endl;
    switch (_order+1) {
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
    setFadeInOne();
}


//--------------------------------------------------------------
void vhpWindow::update(){
    (*this.*currentUpdate)();
}
void vhpWindow::updateFadeIn(){
    if (alpha==255) setVisible();
    setAlpha();
}
void vhpWindow::updateFadeInOne(){
    if (alpha==255) setVisibleOne();
    setAlpha();
}
void vhpWindow::updateVisible(){
    if (ofGetElapsedTimeMillis()>=(start + visibleDuration)) setFadeOut();
}
void vhpWindow::updateVisibleOne(){
    if (ofGetElapsedTimeMillis()>=(start + visibleDuration)) setFadeOutOne();
}
void vhpWindow::updateFadeOut(){
    if (alpha==0) setHidden();
    setAlpha();
}
void vhpWindow::updateFadeOutOne(){
    if (alpha==0) setHiddenOne();
    setAlpha();
}
void vhpWindow::updateHidden(){
    if ((order<4)&&(ofGetElapsedTimeMillis()>=(start + hiddenDuration))) setFadeIn();
}
void vhpWindow::updateHiddenOne(){
    
}

//--------------------------------------------------------------

void vhpWindow::draw(){
    (*this.*currentDraw)();
}
void vhpWindow::drawFadeIn(){
    ofSetColor(255,255,255,alpha);
    window->drawSubsection(wX[0][n], wY, wWidth[0][n], wHeight, wX[0][n], wY, wWidth[0][n], wHeight);
    window->drawSubsection(wX[1][n], wY, wWidth[1][n], wHeight, wX[1][n], wY, wWidth[1][n], wHeight);
}
void vhpWindow::drawFadeInOne(){
    ofSetColor(255,255,255,alpha);
    window->drawSubsection(wX[player][n], wY, wWidth[player][n], wHeight, wX[player][n], wY, wWidth[player][n], wHeight);
}
void vhpWindow::drawVisible(){
    ofSetColor(255,255,255,255);
    window->drawSubsection(wX[0][n], wY, wWidth[0][n], wHeight, wX[0][n], wY, wWidth[0][n], wHeight);
    window->drawSubsection(wX[1][n], wY, wWidth[1][n], wHeight, wX[1][n], wY, wWidth[1][n], wHeight);
}
void vhpWindow::drawVisibleOne(){
    ofSetColor(255,255,255,255);
    window->drawSubsection(wX[player][n], wY, wWidth[player][n], wHeight, wX[player][n], wY, wWidth[player][n], wHeight);
}
void vhpWindow::drawFadeOut(){
    ofSetColor(255,255,255,alpha);
    window->drawSubsection(wX[0][n], wY, wWidth[0][n], wHeight, wX[0][n], wY, wWidth[0][n], wHeight);
    window->drawSubsection(wX[1][n], wY, wWidth[1][n], wHeight, wX[1][n], wY, wWidth[1][n], wHeight);
}
void vhpWindow::drawFadeOutOne(){
    ofSetColor(255,255,255,alpha);
    window->drawSubsection(wX[player][n], wY, wWidth[player][n], wHeight, wX[player][n], wY, wWidth[player][n], wHeight);
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
void vhpWindow::setFadeInOne(){
    currentUpdate = &vhpWindow::updateFadeInOne;
    currentDraw = &vhpWindow::drawFadeInOne;
    setStart();
    state = fadeIn;
}
void vhpWindow::setVisible(){
    currentUpdate = &vhpWindow::updateVisible;
    currentDraw = &vhpWindow::drawVisible;
    setStart();
    state = visible;
}
void vhpWindow::setVisibleOne(){
    currentUpdate = &vhpWindow::updateVisibleOne;
    currentDraw = &vhpWindow::drawVisibleOne;
    setStart();
    state = visible;
}
void vhpWindow::setFadeOut(){
    currentUpdate = &vhpWindow::updateFadeOut;
    currentDraw = &vhpWindow::drawFadeOut;
    setStart();
    state = fadeOut;
}
void vhpWindow::setFadeOutOne(){
    currentUpdate = &vhpWindow::updateFadeOutOne;
    currentDraw = &vhpWindow::drawFadeOutOne;
    setStart();
    state = fadeOut;
}
void vhpWindow::setHidden(){
    currentUpdate = &vhpWindow::updateHidden;
    currentDraw = &vhpWindow::drawHidden;
    setStart();
    state = hidden;
}
void vhpWindow::setHiddenOne(){
    currentUpdate = &vhpWindow::updateHiddenOne;
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