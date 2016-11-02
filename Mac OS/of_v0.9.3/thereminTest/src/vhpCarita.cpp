#include "vhpCarita.h"

//--------------------------------------------------------------
vhpCarita::vhpCarita():n(0){

}

//--------------------------------------------------------------
void vhpCarita::setup(ofImage* _cara){
    cara = _cara;
    margin = 10;
    xofset = 25;
    currentUpdate = &vhpCarita::fullAnimation;
}
void vhpCarita::setAimationFull(){
    currentUpdate = &vhpCarita::fullAnimation;
}
void vhpCarita::setAimationUp(){
    currentUpdate = &vhpCarita::backUp;
}
void vhpCarita::setAimationDown(){
    currentUpdate = &vhpCarita::backDown;
}

//--------------------------------------------------------------
void vhpCarita::update(){
    (*this.*currentUpdate)();
}
void vhpCarita::none(){

}
void vhpCarita::fullAnimation(){
    n++;
    if (n>15) {
        n = 15;
        currentUpdate = &vhpCarita::none;
    }
}
void vhpCarita::backUp(){
    n--;
    if (n<8) {
        n = 8;
        currentUpdate = &vhpCarita::none;
    }
}
void vhpCarita::backDown(){
    n--;
    if (n<3) {
        n = 3;
        currentUpdate = &vhpCarita::none;
    }
}

//--------------------------------------------------------------
void vhpCarita::draw(int _x, int _y){
    cara->drawSubsection(_x - xofset, _y, 175 - margin*2, 117, 175*n + margin, 0);
    update();
}