#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpLmThread.h"

#define MENUNIVEL               0
#define DESTREZA                1
#define ESPIRITUALIDAD          2
#define ORATORIA                3

class vhpLevelMenu {

	public:
	
		// Constructor
        vhpLevelMenu();
        ~vhpLevelMenu();
		
		/* funciones o métodos */
    
        // Inicializar variables y cargar los archivos
        void setup(vhpLmThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene);
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpLevelMenu::*currentUpdate)();
        void draw(int _x, int _y);
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
        void pause();
    
        // Procesado y actualización
        void playPlayerMenu();
        void pausePlayerMenu();
        void loopPlayerMenu(float _pos);
    
        // Utilidades
        void alert(int _e);
        float getPosition();
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed(float _x, float _y);
    
        /* Variables o propiedades */
    
        // Video de fondo
        ofVideoPlayer       video;
        ofFbo               fbo;
        int                 width;
        int                 height;
    
        // Hilo de control
        vhpLmThread *         controller;
    
        // Estado del juego
        int                 currentScene;
        int                 targetScene;
        int                 state;
        int                 selected;
    
        // notificación de eventos
        static ofEvent<int> levelSelection;
    
    
};
