#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"

class vhpSbThread: public ofThread {

	public:
	
		// constructor
		vhpSbThread();
		
		// funcións ou métodos
        void setup(ofVideoPlayer* _target, float _start, float _end);
        void reset(float _start, float _end);
        void internalReset(float _start, float _end);
        void fadeReset(float _start, float _end, float _stored_start, float _stored_end);
        void start();
        void stop();
        void update();
        void (vhpSbThread::*currentUpdate)();
    
        // video controller
        void loop();
        void fadeLoop();
    
		void draw();
        //int getCount();
    
        ofVideoPlayer *         target;
        //vhpGame *             game;
        float                   video_start;
        float                   video_end;
        float                   stored_start;
        float                   stored_end;
        static ofEvent<int>     timeOut;
        int                     goTo;
    
		
		// Variables ou propiedades
    private:
        void                threadedFunction();
        int                 count;
    
};
