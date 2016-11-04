#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpLine.h"
#include "vhpPetamuti.h"

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
        void setup(int _currentScene, int _targetScene1, int _targetScene2);
        void setupResources(ofImage* _bg, ofImage* _colorBarR, ofImage* _colorBarA, vhpPetamuti* _petamuti);
        void setupFonts(ofTrueTypeFont* _TTF, ofTrueTypeFont* _TTFB, ofTrueTypeFont* _TTFM);
        void getText(string _file);
    
        // Precarga de todos los elementos
        void load();
        void (vhpPlayerMenu::*currentLoad)();
        void loadSingle();
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void init();
        void setOne();
        void setTwo();
        void setFour();
        void setNext();
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpPlayerMenu::*currentUpdate)();
        void updateElements();
        void updateTextLine();
    
        void draw(int _x, int _y);
    
        // Dibujado de elementos gráficos
        void drawTextLine(int _x, int _y, int _alpha);
        void drawChooseButton(int _x, int _y);
        void drawButtons(int _n);
        void drawMenu();
        void drawOne();
        void drawTwo();
        void drawFour();
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
        void pause();
    
        // Procesado y actualización
        void emptyMenu();
        void fadeInText();
        void fadeOutText();
        void fadeInOne();
        void fadeOutOne();
        void fadeInTwo();
        void fadeOutTwo();
        void fadeInFour();
        void fadeOutFour();
    
        // Utilidades
        void alert(int _e);
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed(float _x, float _y);
    
        /* Variables o propiedades */
    
        // elementos gráficos
        vector<vhpLine>             lines;
        int                         count;
        ofTrueTypeFont *            TTF;
        ofTrueTypeFont *            TTFB;
        ofTrueTypeFont *            TTFM;
    
        ofImage                     elegir;
        ofImage                     glow;
        ofImage                     balls;
        ofImage                     bases;
        ofImage                     one;
        ofImage                     two;
        ofImage                     four;
    
        ofImage *                   colorBar[2];
        ofImage *                   bg;
        vhpPetamuti *               petamuti;
    
        float                       alpha;
        float                       alpha_increment;
    
        //loader
        vector<ofImage*>            loadingSilge;
        vector<string>              filesSingle;
        bool                        loaded;
        bool                        loading;

        // Video de fondo
        ofFbo                       fbo;
        ofFbo                       transparent;
        int                         width;
        int                         height;
    
        // Estado del juego
        int                         currentScene;
        int                         targetScene[2];
        int                         state;
        int                         target;
        int                         selected;
    
        // notificación de eventos
        static ofEvent<int>         playersNumber;
};
