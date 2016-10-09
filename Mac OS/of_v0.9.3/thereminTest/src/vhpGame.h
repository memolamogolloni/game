#pragma once

#include "ofMain.h"

#include "ofEvents.h"
#include "ofxXmlSettings.h"
#include "vhpOSC.h"

#include "vhpScreenSaver.h"
#include "vhpPlayerMenu.h"
#include "vhpPmThread.h"
#include "vhpStandby.h"
#include "vhpSbThread.h"
#include "vhpLevelMenu.h"
#include "vhpLmThread.h"

#define SCREENSAVER         0
#define PLAYERMENU          1
#define STANDBY             2
#define LEVELMENU           3
#define GAME                4

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
    
        // loading
        void updateLoading();
        void drawLoading();
    
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
    
    // PlayerMenu
    void initLevelMenu();
    void stopLevelMenu();
    void goToLevelMenu();
    void fadeInLevelMenu();
    void fadeOutLevelMenu();
    void updateLevelMenu();
    void updateLevelMenuInOut();
    void drawLevelMenu();
    void drawLevelMenuIn();
    void drawLevelMenuOut();
    
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
    
        vhpOSC                      mensajeria;
        vhpScreenSaver              logos;
        bool                        logosLoaded;
        vhpPlayerMenu               xogadores;
        bool                        xogadoresLoaded;
        vhpPmThread                 controlXogadores;
        vhpStandby                  espera;
        bool                        esperaLoaded;
        vhpSbThread                 controlEspera;
        vhpLevelMenu                niveis;
        bool                        niveisLoaded;
        vhpLmThread                 controlNiveis;
    
    
        /*
         vector<vhpVid>            background;
         vector<ofVideoPlayer> 		windowA;
         vector<ofVideoPlayer> 		windowB;
         vhpButton                   start;
         */
    
};
