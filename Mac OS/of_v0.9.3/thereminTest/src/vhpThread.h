#pragma once

#include "ofMain.h"
#include "vhpVid.h"

#define LOOP            0
#define PAUSE           1

class vhpThread: public ofThread {

	public:
	
		// constructor
		vhpThread();
		
		// funcións ou métodos
        void setup(ofVideoPlayer* _target, float _start, float _end);
        void reset(float _start, float _end);
        void internalReset(float _start, float _end);
        void fadeReset(float _start, float _end, float _stored_start, float _stored_end);
        void start();
        void stop();
        void update();
        void (vhpThread::*currentUpdate)();
    
        // video controller
        void loop();
        void fadeLoop();
    
		void draw();
        //int getCount();
    
        ofVideoPlayer *     target;
        //int               state;
        float               video_start;
        float               video_end;
        float               stored_start;
        float               stored_end;
		
		// Variables ou propiedades
    private:
        void                threadedFunction();
        //int                 count;
    
};
