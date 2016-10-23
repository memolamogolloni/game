#pragma once
#include "ofMain.h"

#define hidden          0
#define fadeIn          1
#define visible         2
#define fadeOut         3

class vhpWindow {

	public:
	
		// constructor
		vhpWindow();
		
		// funcións ou métodos
		void setup();
        void setWindows(ofImage* _purple, ofImage* _yellow, ofImage* _blue, ofImage* _green);
        void setActualWindow();
    
        void update();
        void (vhpWindow::*currentUpdate)();
        void updateFadeIn();
        void updateVisible();
        void updateFadeOut();
        void updateHidden();
    
        void draw();
        void (vhpWindow::*currentDraw)();
        void drawFadeIn();
        void drawVisible();
        void drawFadeOut();
        void drawHidden();
    
        void setFadeIn();
        void setVisible();
        void setFadeOut();
        void setHidden();
    
        void setStart();
        void setAlpha();
    
        // Variables ou propiedades
        float           fadeDuration;
        float           visibleDuration;
        float           hiddenDuration;
        float           start;
        int             order;
        int             state;
        ofImage*        window;
        ofImage*        purple;
        ofImage*        yellow;
        ofImage*        blue;
        ofImage*        green;
        int             alpha;
		
};
