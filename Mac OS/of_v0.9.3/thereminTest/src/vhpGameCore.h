#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpOSC.h"
#include "vhpLine.h"
#include "vhpWindow.h"
#include "vhpCarita.h"

#define nWINDOWS        7
#define nROUNDS         3

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
        void setup(vhpOSC* _mensajeria, int _currentScene, int _targetScene);
        void setupResources(vhpCarita* _roja, vhpCarita* _azul);
        void getText(string _file, bool _string);
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
        void updateTextLine();
    
        void draw(int _x, int _y);
    
        // Dibujado de elementos
        void drawScore();
        void drawBackground();
        void drawGame();
        void drawWindows();
        void drawRound();
        void drawPatternText();
        void drawRoundWiner();
        void drawReadyButton();
        void drawClickedWindow();
        void drawTextLine(int _x, int _y, int _alpha);
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
        void pause();
    
        // Procesado y actualización
        void setRoundTutorial();
        void showRoundTutorial();
        void setRound();
        void playReady();
        void playSteady();
        void playGo();
        void showWindow();
        void showWinner();
        void showTie();
        void setPatternTutorial();
        void showPatternTutorial();
        void setWindowPattern();
        void sendWindowPattern();
        void showPattern();
        void setFinalWinner();
        void showFinalWinner();
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed(float & _x, float & _y);
        void (vhpGameCore::*currentTouchPressed)(float & _x, float & _y);
        void touchPressedNull(float & _x, float & _y);
        void touchPressedRoundTutorial(float & _x, float & _y);
        void touchPressedGame(float & _x, float & _y);
        void touchPressedWinner(float & _x, float & _y);
        void touchPressedPatternTutorial(float & _x, float & _y);
        void touchPressedPattern(float & _x, float & _y);
        void touchPressedPatternWinner(float & _x, float & _y);
    
        void checkIsGo(int _p);
        void checkRoundWinner();
        void checkPatternWinner();
        void triggerPressedGame();
        void triggerPressedPattern();
        void setDelay();
    
        int randomWindow();
        void randomPattern();
        void setTimeReference();
        void setTimeReference(float _ago);
        void setIATimeReference();
        void setIATimeReference(float _ago);
        float getElapsedtime();
        float getIAElapsedtime();
    
    
        /* Variables o propiedades */
    
        vhpOSC*                      mensajeria;
    
        // Video de fondo
        vector<ofVideoPlayer>       video;
        vector<string>              videoFile;
        ofFbo                       fbo;
        int                         width;
        int                         height;
    
        // Elementos gráficos
        vector<string>              lines;
        vector<vhpLine>             fLines;
        int                         count;
        ofTrueTypeFont              TTF;
        ofTrueTypeFont              TTFB;
        ofTrueTypeFont              TTFM;
    
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
        vhpWindow                   aWindowClick[4];
        vhpWindow                   bWindowClick[4];
    
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
        ofImage                     keko;
        ofImage                     glow;
        ofImage                     shadowblue;
        ofImage                     shadowred;
        ofImage                     buttonblue;
        ofImage                     buttonred;
        ofImage                     colorBar[2];
        ofImage                     trofeo;
        ofImage                     bandera;
        ofImage                     ventana;
        ofImage                     afterPurple;
        ofImage                     afterYellow;
        ofImage                     afterGreen;
        ofImage                     afterBlue;
        vhpCarita*                  caritas[2];
    
        // Estado del juego
        int                         level;
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
        vector<int>                 nums;
        int                         registeredPattern[2][4];
        int                         currentRound;
        int                         currentWindow;
        int                         delay;
    
        float                       tRef;
    
        // Recorte de las ventanas
        int                         wHeight;
        int                         wWidth[2][nWINDOWS];
        int                         wY;
        int                         wX[2][nWINDOWS];
    
        float                       angle;
    
    
        // IA
        bool                        IA;
        float                       IAdelay;
        float                       maxIAdelay;
        float                       IAtRef;
    
        // notificación de eventos
        static ofEvent<int> onClick;
        static ofEvent<int> onRestart;
    
};
