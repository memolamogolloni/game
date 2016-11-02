#include "vhpLoader.h"

//--------------------------------------------------------------
vhpLoader::vhpLoader():loaded(false),loading(false){
    
}

//--------------------------------------------------------------
void vhpLoader::setup(){
    imageFile.push_back("caraAzul");
    imageFile.push_back("caraRosa");
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
                if (images[actual].isAllocated()) {
                    cout << "actual: " << actual << " is allocated."<< endl;
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
            loaded = true;
        }
    }
}