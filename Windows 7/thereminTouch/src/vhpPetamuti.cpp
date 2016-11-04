#include "vhpPetamuti.h"

//--------------------------------------------------------------
vhpPetamuti::vhpPetamuti():n(0){

}

//--------------------------------------------------------------
void vhpPetamuti::setup(ofImage* _petamuti){
    petamuti = _petamuti;
    width = 289;
    height = 480;
    margin = 10;
    initialxOfset = 25;
    xofset = 41;
    count = 0;
    currentUpdate = &vhpPetamuti::fullAnimation;
}
void vhpPetamuti::setAimationFull(){
    currentUpdate = &vhpPetamuti::fullAnimation;
}
void vhpPetamuti::setAimationUp(){
    currentUpdate = &vhpPetamuti::backUp;
}
void vhpPetamuti::setAimationDown(){
    currentUpdate = &vhpPetamuti::backDown;
}

//--------------------------------------------------------------
void vhpPetamuti::update(){
    (*this.*currentUpdate)();
}
void vhpPetamuti::none(){

}
void vhpPetamuti::fullAnimation(){
    count ++;
    if (count%3==0){
        n++;
        if (n>16) {
            n = 0;
        }
    }
}
void vhpPetamuti::backUp(){
    n--;
    if (n<8) {
        n = 8;
        currentUpdate = &vhpPetamuti::none;
    }
}
void vhpPetamuti::backDown(){
    n--;
    if (n<3) {
        n = 3;
        currentUpdate = &vhpPetamuti::none;
    }
}

//--------------------------------------------------------------
void vhpPetamuti::draw(int _x, int _y){
    petamuti->drawSubsection(_x - margin, _y, width + margin*2, height, (width+xofset)*n + initialxOfset - margin, 0);
    update();
}