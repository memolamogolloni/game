#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpThread.h"

#define MENU                0
#define ONEPLAYER           1
#define TWOPLAYERS          2
#define FOURPLAYERS         3

class vhpPlayerMenu {

	public:
	
		// Constructor
        vhpPlayerMenu();
        ~vhpPlayerMenu();
		
		/* funciones o métodos */
    
        // Inicializar variables y cargar los archivos
        void setup(vhpThread* _controller, ofxXmlSettings& _videoList, string _videoTag, int _currentScene, int _targetScene);
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpPlayerMenu::*currentUpdate)();
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
        void mouseMoved(ofMouseEventArgs & _args);
        void mouseDragged(ofMouseEventArgs & _args);
        void mousePressed(ofMouseEventArgs & _args);
        void mouseReleased(ofMouseEventArgs & _args);
        void mouseScrolled(ofMouseEventArgs & _args);
        void mouseEntered(ofMouseEventArgs & _args);
        void mouseExited(ofMouseEventArgs & _args);
    
        /* Variables o propiedades */
    
        // Video de fondo
        ofVideoPlayer       video;
        ofFbo               fbo;
        int                 width;
        int                 height;
    
        // Hilo de control
        vhpThread *         controller;
    
        // Estado del juego
        int                 currentScene;
        int                 targetScene;
        float               scale;
        int                 state;

    protected:
        bool registerEvents;
    
};
