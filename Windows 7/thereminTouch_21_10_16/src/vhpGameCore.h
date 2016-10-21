#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpGcThread.h"

#define nWINDOWS        7
#define nROUNDS         4

#define lostW           0
#define wonW            1
#define pendingW        2

class vhpGameCore {

	public:
	
		// Constructor
        vhpGameCore();
        ~vhpGameCore();
		
		/* funciones o métodos */
    
        // Inicializar variables y cargar los archivos
        void setup(vhpGcThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene);
        void initGame();
        void initRound();
                   
        // Comenzar e interrumpir los hilos y listeners de la escena
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpGameCore::*currentUpdate)();
        void draw(int _x, int _y);
    
        // Dibujado de elementos
        void drawScore();
        void drawGame();
    
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
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed(float & _x, float & _y);
        void (vhpGameCore::*currentTouchPressed)(float & _x, float & _y);
        void touchPressedGame(float & _x, float & _y);
        void touchPressedWinner(float & _x, float & _y);
    
        /* Variables o propiedades */
    
        // Video de fondo
        ofVideoPlayer       video;
        ofFbo               fbo;
        int                 width;
        int                 height;
    
        // Elementos gráficos
        int                 alpha;
        int                 alphaWindow[2];
        int                 alpha_increment;
        ofImage             ready;
        ofImage             steady;
        ofImage             go;
        ofImage             building;
        ofImage             windowA[nWINDOWS];
        ofImage             windowB[nWINDOWS];
        int                 windowState[2][7];
        ofImage             windowShowA[nWINDOWS];
        ofImage             windowShowB[nWINDOWS];
        ofImage             winnerBackground[2];
        ofImage             winnerButton[2];
        ofImage             score[2];
        ofTrueTypeFont      TTF;
    
        // Estado del juego
        int             currentScene;
        int             targetScene;
        float           scale;
        bool            hold[2];
        int             clicked[2];
        int             points[2];
        int             time[2];
        bool            ok[2];
        bool            next[2];
        int             winner;
        int             holdSteady;
        int             targetsShot[2];
        int             currentRound;
        int             delay;
    
        // notificación de eventos
        static ofEvent<int> onClick;
    
};
