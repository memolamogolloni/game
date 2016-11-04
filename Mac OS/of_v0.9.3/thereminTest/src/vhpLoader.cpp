#include "vhpLoader.h"

//--------------------------------------------------------------
vhpLoader::vhpLoader():loaded(false),loading(false){
    
}

//--------------------------------------------------------------
void vhpLoader::setup(){
    
    imageFile.push_back("flecha-secuencia");
    imageFile.push_back("petamuti-secuencia");
    imageFile.push_back("t-blue-b");
    imageFile.push_back("t-red-b");
    imageFile.push_back("t-bg");
    imageFile.push_back("caraAzul");
    imageFile.push_back("caraRosa");
    
    // Añadir las fuentes
    TTF.load("fonts/titilliumweblight.ttf", 22);
    TTF.setGlobalDpi(72);
    TTFB.load("fonts/titilliumweblight.ttf", 70);
    TTFB.setGlobalDpi(72);
    TTFM.load("fonts/titilliumweblight.ttf", 55);
    TTFM.setGlobalDpi(72);
    
    currentLoad = &vhpLoader::loadImages;
    
}

//--------------------------------------------------------------
void vhpLoader::update(){

}

//--------------------------------------------------------------
void vhpLoader::draw(){

}

// Precarga de todos los elementos -----------------------------
void vhpLoader::load(){
    (*this.*currentLoad)();
}
void vhpLoader::loadImages(){
    if (!loaded) {
        int actual = imageFile.size() - 1;
        cout << "actual: " << actual << endl;
        if (actual>=0) {
            if (loading) {
                // cout << "loading: " << actual << endl;
                if (images[images.size()-1].isAllocated()) {
                    cout << "actual: " << images.size()-1 << " is allocated."<< endl;
                    cout << imageFile.size() << " pending!"<< endl;
                    loading = false;
                }
            } else {
                images.push_back(ofImage());
                images[images.size()-1].load("images/"+ imageFile[actual] +".png");
                cout << "load images/"+ imageFile[actual] +".png" << endl;
                imageFile.pop_back();
                loading = true;
            }
        } else {
            cout << "loading single images in Player Menu finished!" << endl;
            caritas[0].setup(&images[0]);
            caritas[1].setup(&images[1]);
            petamuti.setup(&images[5]);
            flecha.setup(&images[6]);
            loaded = true;
        }
    }
}