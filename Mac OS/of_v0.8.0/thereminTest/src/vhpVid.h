#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"

class vhpVid {

	public:
	
		// constructor
		vhpVid();
		
		// funcións ou métodos
        void setup();
        void setup(string _file);
        void addVideos(ofxXmlSettings& _videoList, string _backTag, string _frontTag);
        void update();
        void (vhpVid::*currentUpdate)();
        void backPlay();
        void frontPlay();
        void frontFadeIn();
        void frontFadeout();
        void loop(float _pos);
        void pause();
        void setPlay();
        void setPause();
        void setFrontFadeIn();
        void draw(int _x, int _y);
    
        float getPosition();

        // Variables ou propiedades
        ofVideoPlayer   backVideo;
        ofVideoPlayer   frontVideo;
        vector<string>  backVideoList;
        vector<string>  frontVideoList;
        ofFbo           fbo;
        ofShader        shaderMixture;
        ofTexture       bufferTex;
        int             width;
        int             height;
        int             alpha;
        int             alpha_increment;
		
};
