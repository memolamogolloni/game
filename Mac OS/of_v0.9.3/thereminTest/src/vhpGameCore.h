#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpOSC.h"
#include "vhpWindow.h"

#define nWINDOWS        7
#define nROUNDS         4

#define lostW           0
#define wonW            1
#define pendingW        2
#define whiteW          3

class vhpGameCore {

	public:
	
		// Constructor
        vhpGameCore();
        ~vhpGameCore();
		
		/* funciones o métodos */
    
        // Inicializar variables y cargar los archivos
        void setup(ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene);
        void getText(string _file);
        void initGame();
        void initRound();
        void initPattern();
    
        // Precarga de todos los elementos
        void load();
        void (vhpGameCore::*currentLoad)();
        void loadVideo();
        void loadSingle();
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpGameCore::*currentUpdate)();
        void draw(int _x, int _y);
    
        // Dibujado de elementos
        void drawScore();
        void drawBackground();
        void drawGame();
        void drawWindows();
        void drawRound();
        void drawRoundWiner();
        void drawClickedWindow();
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
        void pause();
    
        // Procesado y actualización
        void playScreenSaver();
        void pauseScreenSaver();
        void loopScreenSaver(float _pos);
        void playReady();
        void playSteady();
        void playGo();
        void showWindow();
        void showWinner();
        void showFinalWinner();
        void showTie();
        void setWindowPattern();
        void showPattern();
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed(float & _x, float & _y);
        void (vhpGameCore::*currentTouchPressed)(float & _x, float & _y);
        void touchPressedGame(float & _x, float & _y);
        void touchPressedWinner(float & _x, float & _y);
        void touchPressedPattern(float & _x, float & _y);
        void touchPressedPatternWinner(float & _x, float & _y);
    
        void checkIsGo(int _p);
    
        int randomWindow();
        void randomPattern();
        void setTimeReference();
        float getElapsedtime();
    
        /* Variables o propiedades */
    
        vhpOSC                      mensajeria;
    
        // Video de fondo
        vector<ofVideoPlayer>       video;
        vector<string>              videoFile;
        ofFbo                       fbo;
        int                         width;
        int                         height;
    
        // Elementos gráficos
        vector<string>              lines;
        int                         count;
        ofTrueTypeFont              TTF;
        ofTrueTypeFont              TTFB;
    
        int                         alpha;
        int                         alphaWindow[4];
        int                         alpha_increment;
        ofImage                     tie;
        ofImage                     building;
    
    
        vector<ofImage*>            loadingSilge;
        vector<string>              filesSingle;
        bool                        loaded;
        bool                        loading;
    
        vhpWindow                   pWindow;
    
        int                         windowState[2][7];
        ofImage                     winnerBackground[2];
        ofImage                     winnerButton[2];
        ofImage                     score[2];
        ofImage                     purple;
        ofImage                     yellow;
        ofImage                     blue;
        ofImage                     green;
        ofImage                     iconosR;
        ofImage                     iconosA;
        ofImage                     marcos;
        ofImage                     avisos;
        ofImage                     click;
        ofImage                     bg;
        ofImage                     balls;
        ofImage                     shadow;
        ofImage                     wrong;
    
        // Estado del juego
        int                         currentScene;
        int                         targetScene;
        float                       scale;
        bool                        hold[2];
        bool                        isGo;
        int                         clicked[2];
        int                         points[2];
        int                         time[2];
        bool                        ok[2];
        bool                        soon[2];
        bool                        next[2];
        int                         winner;
        int                         holdSteady;
        int                         targetsShot;
        int                         targetsPattern[4];
        int                         registeredPattern[2][4];
        int                         currentRound;
        int                         delay;
    
        float                       tRef;
    
        // Recorte de las ventanas
        int             wHeight;
        int             wWidth[2][nWINDOWS];
        int             wY;
        int             wX[2][nWINDOWS];
    
        // notificación de eventos
        static ofEvent<int> onClick;
    
};
