#include "vhpFlecha.h"

//--------------------------------------------------------------
vhpFlecha::vhpFlecha():n(0){

}

//--------------------------------------------------------------
void vhpFlecha::setup(ofImage* _flecha){
    num = 0;
    flecha = _flecha;
    width = 165;
    height = 176;
    margin = 0;
    initialxOfset = 1;
    xofset = 0;
    count = 0;
    currentUpdate = &vhpFlecha::fullAnimation;
}
void vhpFlecha::setAimationFull(){
    num = 0;
    currentUpdate = &vhpFlecha::fullAnimation;
}
void vhpFlecha::setAimationUp(){
    currentUpdate = &vhpFlecha::backUp;
}
void vhpFlecha::setAimationDown(){
    currentUpdate = &vhpFlecha::backDown;
}

//--------------------------------------------------------------
void vhpFlecha::update(){
    (*this.*currentUpdate)();
}
void vhpFlecha::none(){

}
void vhpFlecha::fullAnimation(){
    count ++;
    if (count%2==0){
        n++;
        if (n>19) {
            n = 0;
            num ++;
            if (num==2) currentUpdate = &vhpFlecha::none;
        }
    }
}
void vhpFlecha::backUp(){
    n--;
    if (n<8) {
        n = 8;
        currentUpdate = &vhpFlecha::none;
    }
}
void vhpFlecha::backDown(){
    n--;
    if (n<3) {
        n = 3;
        currentUpdate = &vhpFlecha::none;
    }
}

//--------------------------------------------------------------
void vhpFlecha::draw(int _x, int _y, float _alpha){
    ofSetColor(255, 255, 255, _alpha);
    flecha->drawSubsection(_x - margin, _y, width + margin*2, height, (width+xofset)*n + initialxOfset - margin, 0);
    update();
}