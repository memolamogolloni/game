#pragma once
#include "ofMain.h"

#include "ofEvents.h"

#include "ofxXmlSettings.h"
#include "vhpThread.h"
//#include "VHPbutton.h"
#include "vhpScreenSaver.h"
#include "vhpPlayerMenu.h"
#include "vhpStandby.h"

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
        void drawFrameRate();
        void set(int &_state);
        void toggleScale();
    
        // ScreenSaver
        void initScreenSaver();
        void stopScreenSaver();
        void goToScreenSaver();
        void updateScreenSaver();
        void drawScreenSaver();
    
        // PlayerMenu
        void initPlayerMenu();
        void stopPlayerMenu();
        void goToPlayerMenu();
        void fadeInPlayerMenu();
        void fadeOutPlayerMenu();
        void updatePlayerMenu();
        void updatePlayerMenuInOut();
        void drawPlayerMenu();
        void drawPlayerMenuIn();
        void drawPlayerMenuOut();
    
        // Standby
        void initStandby();
        void stopStandby();
        void goToStandby();
        void fadeInStandby();
        void fadeOutStandby();
        void updateStandby();
        void updateStandbyInOut();
        void drawStandby();
        void drawStandbyIn();
        void drawStandbyOut();
    
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
        vhpThread                   controlXogadores;
        vhpStandby                  espera;
    
        /*
         vector<vhpVid>            background;
         vector<ofVideoPlayer> 		windowA;
         vector<ofVideoPlayer> 		windowB;
         vhpButton                   start;
         */
    
};
