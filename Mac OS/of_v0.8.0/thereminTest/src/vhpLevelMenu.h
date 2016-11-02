#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"
#include "vhpLine.h"
#include "ofxTrueTypeFontUC.h"

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
        void setup(int _currentScene, int _targetScene);
        void setupResources(ofImage* _bg, ofImage* _keko);
        void setupFonts(ofxTrueTypeFontUC* _TTF, ofxTrueTypeFontUC* _TTFB);
        void getText(string _file);
    
        // Precarga de todos los elementos
        void load();
        void (vhpLevelMenu::*currentLoad)();
        void loadSingle();
    
        // Comenzar e interrumpir los hilos y listeners de la escena
        void init();
        void setDestreza();
        void setEspiritualidad();
        void setOratoria();
        void setNext();
        void start();
        void stop();
    
        // Dibujado y actualización variables
        void update();
        void (vhpLevelMenu::*currentUpdate)();
        void updateElements();
        void updateTextLine();
    
        void draw(int _x, int _y);
    
        // Dibujado de elementos gráficos
        void drawTextLine(int _x, int _y, int _alpha);
        void drawMenu();
        void drawDestreza();
        void drawEspiritualidad();
        void drawOratoria();
    
        // reproducir o detener la escena modificando currentUpdate
        void play();
    
        // Procesado y actualización
        void emptyMenu();
        void fadeInText();
        void fadeOutText();
        void fadeInDestreza();
        void fadeOutDestreza();
        void fadeInEspiritualidad();
        void fadeOutEspiritualidad();
        void fadeInOratoria();
        void fadeOutOratoria();
    
        // Utilidades
        void alert(int _e);
    
        // Para recoger la pulsación del ratón en la pantalla
        void touchPressed(float _x, float _y);
    
        /* Variables o propiedades */
    
        // elementos gráficos
        vector<vhpLine>             lines;
        int                         count;
        ofxTrueTypeFontUC *            TTF;
        ofxTrueTypeFontUC *            TTFB;
    
        ofImage                     glow;
        ofImage                     icons;
        ofImage                     bases;
        ofImage                     destreza;
        ofImage                     espiritualidad;
        ofImage                     oratoria;
    
    
        ofImage *                    keko;
        ofImage *                    bg;
    
        int                         alpha;
        int                         alpha_increment;
    
        // fbo
        ofFbo                       fbo;
        int                         width;
        int                         height;
    
        //loader
        vector<ofImage*>            loadingSilge;
        vector<string>              filesSingle;
        bool                        loaded;
        bool                        loading;
    
        // Estado del juego
        int                 currentScene;
        int                 targetScene;
        int                 state;
        int                 target;
        int                 selected;
    
        // notificación de eventos
        static ofEvent<int> levelSelection;
    
    
};
