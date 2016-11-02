#pragma once
#include "ofMain.h"
#include "vhpCarita.h"

class vhpLoader {

	public:
	
		// constructor
		vhpLoader();
		
		// funcións ou métodos
		void setup();
		void update();
		void draw();
		
        // Precarga de todos los elementos
        void load();
        void (vhpLoader::*currentLoad)();
        void loadImages();
    
		// Variables ou propiedades
		
        //loader
        vector<ofImage>             images;
        vector<string>              imageFile;
        bool                        loaded;
        bool                        loading;
    
        vhpCarita                   caritas[2];
};
