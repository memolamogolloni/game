#pragma once
#include "ofMain.h"

#include "ofEvents.h"

#include "ofxXmlSettings.h"
//#include "vhpVid.h"
//#include "VHPbutton.h"
#include "vhpScreenSaver.h"
#include "vhpPlayerMenu.h"

#define SCREENSAVER         0
#define PLAYERMENU          1
#define STANDBY             2
#define FIRSTLEVEL          3
#define SECONDLEVEL         4
#define THIRDLEVEL          5

class vhpGame {

	public:
	
		// constructor
        vhpGame();
        ~vhpGame();
		
		// funcións ou métodos
		void setup();
        void update();
        void (vhpGame::*currentUpdate)();
        void draw();
        void (vhpGame::*currentDraw)();
        void set(int &_state);
        void toggleScale();
    
        // ScreenSaver
        void setScreensaver();
        void updateScreenSaver();
        void drawScreenSaver();
        void stopScreenSaver();
    
        // PlayerMenu
        void setPlayerMenu();
        void updatePlayerMenuFadeIn();
        void updatePlayerMenu();
        void drawPlayerMenuFadeIn();
        void drawPlayerMenu();
    
        // Standby
        void updateStandby();
        void drawStandby();
    
        // FirstLevel
        void updateFirstLevel();
        void drawFirstLevel();
    
        // SecondLevel
        void updateSecondLevel();
        void drawSecondLevel();
    
        // ThirdLevel
        void updateThirdLevel();
        void drawThirdLevel();
    
        // Events
        void keyReleased(int _key);
        void mousePressed(int _x, int _y, int _button);
        void mouseReleased(int x, int y, int button);
    
        // Variables ou propiedades
        ofTrueTypeFont              TTF;
        float                       scale;
        int                         state;
        int                         width;
        int                         height;
        ofxXmlSettings              videoList;
        ofFbo                       fullScreen;
        ofShader                    shaderMixture;
        ofTexture                   bufferTex;
        int                         alpha;
        int                         alpha_increment;
    
        vhpScreenSaver              logos;
        vhpPlayerMenu               xogadores;
    
        /*
         vector<vhpVid>            background;
         vector<ofVideoPlayer> 		windowA;
         vector<ofVideoPlayer> 		windowB;
         vhpButton                   start;
         */
    
};
