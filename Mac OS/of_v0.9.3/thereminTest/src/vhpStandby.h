#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpLine.h"

#define WAITING                 0
#define CANCELED                1
#define ACEPTED                 2

class vhpStandby {

	public:
	
		// Constructor
        vhpStandby();
        ~vhpStandby();
		
		/* funciones o métodos */
    
        // Inicializar variables y cargar los archivos
        void setup(int _currentScene, int _targetScene);
        void getText(string _file);
    
        // Precarga de todos los elementos
        void load();
        void (vhpStandby::*currentLoad)();
        void loadSingle();
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void setWaiting();
        void setAcepted();
        void setCancel();
    
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpStandby::*currentUpdate)();
        void updateElements();
    
        void draw(int _x, int _y);
    
        // Dibujado de elementos gráficos
        void drawWaiting();
        void drawAcepted();
        void drawCancel();
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
        
        // Procesado y actualización
        void fadeInWaiting();
        void fadeOutWaiting();
        void fadeInAcepted();
        void fadeOutAcepted();
        void fadeInCancel();
        //void fadeOutCancel();
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed();
    
        /* Variables o propiedades */
    
        // Video de fondo
        ofFbo                       fbo;
        int                         width;
        int                         height;
    
        // elementos gráficos
        vector<string>              lines;
        int                         count;
        int                         countDown;

        ofTrueTypeFont              TTF;
        ofTrueTypeFont              TTFB;
    
        ofImage                     clock;
        ofImage                     cancel;
        ofImage                     bg;
        ofImage                     balls;
        ofImage                     bases;
        ofImage                     shadow;
        ofImage                     back;
    
        float                       alpha;
        float                       alpha_increment;
    
        //loader
        vector<ofImage*>            loadingSilge;
        vector<string>              filesSingle;
        bool                        loaded;
        bool                        loading;
    
        // Estado del juego
        int                         currentScene;
        int                         targetScene;
        int                         state;
    
        // notificación de eventos
        static ofEvent<int> onCancel;
    
};
