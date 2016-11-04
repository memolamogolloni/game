#pragma once
#include "ofMain.h"

class vhpFlecha {

	public:
	
		// constructor
		vhpFlecha();
		
		// funcións ou métodos
        void setup(ofImage* _flecha);
        void setAimationFull();
        void setAimationUp();
        void setAimationDown();
		
        void update();
        void (vhpFlecha::*currentUpdate)();
        void none();
        void fullAnimation();
        void backUp();
        void backDown();
    
		void draw(int _x, int _y, float _alpha);
		
        /* Variables o propiedades */
    
        ofImage *               flecha;
        int                     n;
        int                     num;
        int                     margin;
        int                     initialxOfset;
        int                     xofset;
        int                     width;
        int                     height;
        int                     count;
		
};
