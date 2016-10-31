#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

class vhpOSC {
    
    public:
    
        // constructor
        vhpOSC();
    
        // funcións ou métodos
        void setup();
        void setOSC(ofxXmlSettings& _settings, string _sender);
        void send(int _players);
        void send(string _address, int _val);
        void start();
        void stop();
        void update();
        void (vhpOSC::*currentUpdate)();
    
        // comunication states
        void receivePlayers();
    
    
        // OSC comunication
        string s_host;
        int s_port;
        int r_port;
        ofxOscReceiver receiver;
        ofxOscSender sender;
    
        int receivedPlayers;
    
        static ofEvent<int> playersReceived;
    
    private:
    
};
