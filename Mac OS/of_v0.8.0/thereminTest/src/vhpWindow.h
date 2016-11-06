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
        void setAfterWindows(ofImage* _apurple, ofImage* _ayellow, ofImage* _ablue, ofImage* _agreen);
        void setActualWindow();
        void setOneWindow(int _order, int _n, int _player);
    
        void update();
        void (vhpWindow::*currentUpdate)();
        void updateFadeIn();
        void updateFadeInOne();
        void updateFadeInIA();
        void updateVisible();
        void updateVisibleOne();
        void updateVisibleIA();
        void updateFadeOut();
        void updateFadeOutOne();
        void updateFadeOutIA();
        void updateHidden();
        void updateHiddenOne();
        void updateHiddenIA();
    
        void draw();
        void (vhpWindow::*currentDraw)();
        void drawFadeIn();
        void drawFadeInOne();

        void drawVisible();
        void drawVisibleOne();
        void drawFadeOut();
        void drawFadeOutIA();
        void drawFadeOutOne();
        void drawHidden();
        void drawHiddenOne();
    
        void setFadeIn();
        void setFadeInOne();
        void setFadeInIA();
        void setVisible();
        void setVisibleOne();
        void setVisibleIA();
        void setFadeOut();
        void setFadeOutOne();
        void setFadeOutIA();
        void setHidden();
        void setHiddenOne();
        void setHiddenIA();
        void setHiddenOneAfter();
    
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
        ofImage*        aWindow;
        ofImage*        purple;
        ofImage*        yellow;
        ofImage*        blue;
        ofImage*        green;
        ofImage*        aPurple;
        ofImage*        aYellow;
        ofImage*        aBlue;
        ofImage*        aGreen;
        int             alpha;
    
        // Recorte de las ventanas
        int             player;
        int             n;
        int             nW[4];
        int             wHeight;
        int             wWidth[2][7];
        int             wY;
        int             wX[2][7];
		
};
