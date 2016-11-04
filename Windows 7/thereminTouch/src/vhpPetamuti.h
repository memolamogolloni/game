#pragma once
#include "ofMain.h"

class vhpPetamuti {

	public:
	
		// constructor
		vhpPetamuti();
		
		// funcións ou métodos
        void setup(ofImage* _petamuti);
        void setAimationFull();
        void setAimationUp();
        void setAimationDown();
		
        void update();
        void (vhpPetamuti::*currentUpdate)();
        void none();
        void fullAnimation();
        void backUp();
        void backDown();
    
		void draw(int _x, int _y);
		
        /* Variables o propiedades */
    
        ofImage *               petamuti;
        int                     n;
        int                     margin;
        int                     initialxOfset;
        int                     xofset;
        int                     width;
        int                     height;
        int                     count;
		
};
