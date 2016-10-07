#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

#define LOOP            0
#define PAUSE           1

class vhpStandbyThread: public ofThread {

	public:
	
		// constructor
		vhpStandbyThread();
		
		// funcións ou métodos
        void setup(ofVideoPlayer* _target, float _start, float _end);
        void setOSC(ofxXmlSettings& _settings);
        void send(int _players);
        void reset(float _start, float _end);
        void internalReset(float _start, float _end);
        void fadeReset(float _start, float _end, float _stored_start, float _stored_end);
        void start();
        void stop();
        void update();
        void (vhpStandbyThread::*currentUpdate)();
    
        // video controller
        void loop();
        void fadeLoop();
    
		void draw();
        //int getCount();
    
        // OSC comunication
        string s_host;
        int s_port;
        int r_port;
        ofxOscReceiver receiver;
        ofxOscSender sender;
    
        /// port ip
        // notificate  
        //
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
