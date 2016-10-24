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
        void setWindows(ofImage* _purple, int _nP, ofImage* _yellow, int _nY, ofImage* _blue, int _nB, ofImage* _green, int _nG);
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
    
        // Recorte de las ventanas
        int             n;
        int             nW[4];
        int             wHeight;
        int             wWidth[2][7];
        int             wY;
        int             wX[2][7];
		
};
