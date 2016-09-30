#include "VHPbutton.h"

//constructor

//----------------------------------------------------------------	
vhpButton::vhpButton() : bRegisteredEvents(false), position(0, 0), radius(DEFAULT_SIZE*0.5), squareRadius((DEFAULT_SIZE*0.5+OVER_MARGIN)*(DEFAULT_SIZE*0.5+OVER_MARGIN)), scale(1.0){
    init();
}
vhpButton::~vhpButton(){
    if(bRegisteredEvents) {
        ofUnregisterMouseEvents(this); // disable litening to mous events.
        bRegisteredEvents = false;
    }
}
vhpButton::vhpButton(float _x, float _y, int _r) : bRegisteredEvents(false), position(_x, _y), radius(_r), squareRadius((_r+OVER_MARGIN)*(_r+OVER_MARGIN)), scale(1.0){
    init();
}

// methods

//----------------------------------------------------------------	

void vhpButton::init() {
    if(!bRegisteredEvents) {
        ofRegisterMouseEvents(this); // this will enable our circle class to listen to the mouse events.
        bRegisteredEvents = true;
    }
    active = false;
    target = 1;
    lineColor.set(180, 180, 180, 120);
    fillColor.set(255, 255, 255, 255);
    activeColor.set(180, 180, 180, 80);
}

//----------------------------------------------------------------	

void vhpButton::set(bool _a) {
    active = _a;
}
void vhpButton::setPosition(float _x, float _y) {
    position.set(_x, _y);
}
void vhpButton::setRadius(int _r) {
    radius = _r;
    squareRadius = (_r+OVER_MARGIN)*(_r+OVER_MARGIN);
}

void vhpButton::toggle() {
    active = !active;
}

//----------------------------------------------------------------

void vhpButton::display() {
    ofSetCircleResolution(100);
    ofFill();
    if (active) {
        ofSetColor(activeColor);
    } else {
        ofSetColor(fillColor);
    }
    ofCircle(position.x, position.y, radius);
    ofNoFill();
    ofSetLineWidth(1.5);
    ofSetColor(lineColor);
    ofCircle(position.x, position.y, radius);
}
void vhpButton::display(float _a) {
    ofFill();
    if (active) {
        ofSetColor(activeColor.r, activeColor.g, activeColor.b, activeColor.a* _a);
    } else {
        ofSetColor(fillColor.r, fillColor.g, fillColor.b, fillColor.a* _a);
    }
    ofCircle(position.x, position.y, radius);
    ofNoFill();
    ofSetLineWidth(1.5);
    ofSetColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a* _a);
    ofCircle(position.x, position.y, radius);
}

//----------------------------------------------------------------

bool vhpButton::isOver(const float& _x, const float& _y) {
    cout << "mouse x: " << _x << " mouse y: " << _y << endl;
    cout << "button x: " << position.x << " button y: " << position.y << endl;
    if (squareRadius>=squareDistance(_x, _y)) {
        return true;
    } else {
        return false;
    }
}

float vhpButton::squareDistance(float _x, float _y) const {
    float vx = position.x - _x;
    float vy = position.y - _y;
    return vx * vx + vy * vy;
}

//--------------------------------------------------------------
void vhpButton::mouseMoved(ofMouseEventArgs & _args){}
void vhpButton::mouseDragged(ofMouseEventArgs & _args){}
void vhpButton::mousePressed(ofMouseEventArgs & _args){
    if (isOver(_args.x*3/scale, _args.y*3/scale)) {
        toggle();
    }
}
void vhpButton::mouseReleased(ofMouseEventArgs & _args){
    if (active) {
        cout << "Button active!" << endl;
        ofNotifyEvent(onActive, target);
        toggle();
    }
}
void vhpButton::mouseScrolled(ofMouseEventArgs & _args){}
void vhpButton::mouseEntered(ofMouseEventArgs & _args){}
void vhpButton::mouseExited(ofMouseEventArgs & _args){}

ofEvent <int> vhpButton::onActive;