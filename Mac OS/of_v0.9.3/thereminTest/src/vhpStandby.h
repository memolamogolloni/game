#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpSbThread.h"

class vhpStandby {

	public:
	
		// Constructor
        vhpStandby();
        ~vhpStandby();
		
		/* funciones o métodos */
    
        // Inicializar variables y cargar los archivos
        void setup(vhpSbThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene);
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpStandby::*currentUpdate)();
        void draw(int _x, int _y);
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
        void pause();
    
        // Procesado y actualización
        void playScreenSaver();
        void pauseScreenSaver();
        void loopScreenSaver(float _pos);
    
        // Para recoger la pulsación del ratón en la pantalla
        void mouseMoved(ofMouseEventArgs & _args);
        void mouseDragged(ofMouseEventArgs & _args);
        void mousePressed(ofMouseEventArgs & _args);
        void mouseReleased(ofMouseEventArgs & _args);
        void mouseScrolled(ofMouseEventArgs & _args);
        void mouseEntered(ofMouseEventArgs & _args);
        void mouseExited(ofMouseEventArgs & _args);
    
        /* Variables o propiedades */
    
        // Video de fondo
        ofVideoPlayer   video;
        ofFbo           fbo;
        int             width;
        int             height;
    
        // Hilo de control
        vhpSbThread *         controller;
    
        // Estado del juego
        int             currentScene;
        int             targetScene;
    
        // notificación de eventos
        static ofEvent<int> onClick;
    
    protected:
        bool registerEvents;
    
};
