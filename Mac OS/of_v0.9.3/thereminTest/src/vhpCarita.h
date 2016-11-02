#pragma once
#include "ofMain.h"

class vhpCarita {

	public:
	
		// constructor
		vhpCarita();
		
		// funcións ou métodos
        void setup(ofImage* _cara);
        void setAimationFull();
        void setAimationUp();
        void setAimationDown();
		
        void update();
        void (vhpCarita::*currentUpdate)();
        void none();
        void fullAnimation();
        void backUp();
        void backDown();
    
		void draw(int _x, int _y);
		
        /* Variables o propiedades */
    
        ofImage*                cara;
        int                     n;
        int                     margin;
        int                     xofset;
		
};
