#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"

class vhpScreenSaver {

	public:
	
		// Constructor
        vhpScreenSaver();
        ~vhpScreenSaver();
		
		/* funciones o métodos */
    
        // Inicializar variables y cargar los archivos
        void setup(ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene);
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpScreenSaver::*currentUpdate)();
        void draw(int _x, int _y);
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
        void pause();
    
        // Procesado y actualización
        void playScreenSaver();
        void pauseScreenSaver();
        void loopScreenSaver(float _pos);
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed();
    
        /* Variables o propiedades */
    
        // Video de fondo
        ofVideoPlayer   video;
        ofFbo           fbo;
        int             width;
        int             height;
    
        // Estado del juego
        int             currentScene;
        int             targetScene;
    
        // notificación de eventos
        static ofEvent<int> onClick;
    
};
