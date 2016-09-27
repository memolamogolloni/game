#pragma once

#include "ofMain.h"
#include "ofEvents.h"

#define OVER_MARGIN		4
#define DEFAULT_SIZE    14

class vhpButton {
    public:
    
        //constructor
        vhpButton();
        ~vhpButton();
        vhpButton(float _x, float _y, int _r);
    
        // methods
        void init();
        void set(bool _a);
        void setPosition(float _x, float _y);
        void setRadius(int _r);
        void toggle();
        void display();
        void display(float _a);
        float squareDistance(float _x, float _y) const;
        bool isOver(const float& _x, const float& _y);
    
        //We need to declare all this mouse events methods to be able to listen to mouse events.
        //All this must be declared even if we are just going to use only one of this methods.
        void mouseMoved(ofMouseEventArgs & _args);
        void mouseDragged(ofMouseEventArgs & _args);
        void mousePressed(ofMouseEventArgs & _args);
        void mouseReleased(ofMouseEventArgs & _args);
        void mouseScrolled(ofMouseEventArgs & _args);
        void mouseEntered(ofMouseEventArgs & _args);
        void mouseExited(ofMouseEventArgs & _args);
    
        // variables
        float scale;
        ofVec2f position;
        bool active;
        ofColor lineColor;
        ofColor fillColor;
        ofColor activeColor;
        float radius;
        float squareRadius;
        ofVec2f center;
    
        static ofEvent<int> onActive;
        int target;
    
    protected:
        bool bRegisteredEvents;
	
};
