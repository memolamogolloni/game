#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"

class vhpScreenSaver {

	public:
	
		// constructor
        vhpScreenSaver();
        ~vhpScreenSaver();
		
		// funcións ou métodos
        void setup(string _file);
        void setId(int _id, int _target);
        void addVideos(ofxXmlSettings& _videoList, string _videoTag);
        void init();
        void stop();
        void update();
        void (vhpScreenSaver::*currentUpdate)();
        void play();
        void loop(float _pos);
        void pause();
        void setPlay();
        void setPause();
        void draw(int _x, int _y);
    
        float getPosition();
    
        //We need to declare all this mouse events methods to be able to listen to mouse events.
        //All this must be declared even if we are just going to use only one of this methods.
        void mouseMoved(ofMouseEventArgs & _args);
        void mouseDragged(ofMouseEventArgs & _args);
        void mousePressed(ofMouseEventArgs & _args);
        void mouseReleased(ofMouseEventArgs & _args);
        void mouseScrolled(ofMouseEventArgs & _args);
        void mouseEntered(ofMouseEventArgs & _args);
        void mouseExited(ofMouseEventArgs & _args);
    
        // Variables ou propiedades
        ofVideoPlayer   video;
        vector<string>  videoList;
        ofFbo           fbo;
        int             width;
        int             height;
        int             gameId;
        int             gameTarget;

        static ofEvent<int> onClick;
    
    protected:
        bool registerEvents;
    
};
