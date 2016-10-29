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
        void setup(int _currentScene, int _targetScene);
    
        // Precarga de todos los elementos
        void load();
        void (vhpScreenSaver::*currentLoad)();
        void loadSingle();

    
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
        ofFbo                       fbo;
        int                         width;
        int                         height;
    
        ofImage                     bg;
        ofImage                     theremin;
        ofImage                     loadingtxt;
        float                       alpha;
        float                       alpha_increment;
        float                       salpha;
        float                       salpha_increment;
        bool                        finishLoading;
    
        //loader
        vector<ofImage*>            loadingSilge;
        vector<string>              filesSingle;
        bool                        loaded;
        bool                        loading;
    
    
        // Estado del juego
        int                         currentScene;
        int                         targetScene;
    
        // notificación de eventos
        static ofEvent<int>         onClick;
    
};
