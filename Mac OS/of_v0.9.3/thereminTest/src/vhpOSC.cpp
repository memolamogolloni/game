#include "vhpOSC.h"

//--------------------------------------------------------------
vhpOSC::vhpOSC() {
    
}

//--------------------------------------------------------------
void vhpOSC::setup(){
    
}
void vhpOSC::setOSC(ofxXmlSettings& _settings){
    
    // Añadir LOS PUERTOS Y EL HOST desde el documento xml de settings
    int n = _settings.getNumTags("SENDER");
    if(n > 0){
        _settings.pushTag("SENDER", n-1);
        int num = _settings.getNumTags("PORT");
        cout << num << " ports: " << _settings.getValue("PORT", 8000, 0) << endl;
        s_port = _settings.getValue("PORT", 8000, 0);
        num = _settings.getNumTags("HOST");
        cout << num << " hosts: " << _settings.getValue("HOST", "localhost", 0) << endl;
        s_host = _settings.getValue("HOST", "localhost", 0);
        _settings.popTag();
    }
    n = _settings.getNumTags("RECEIVER");
    if(n > 0){
        _settings.pushTag("RECEIVER", n-1);
        int num = _settings.getNumTags("PORT");
        cout << num << " ports: " << _settings.getValue("PORT", 8000, 0) << endl;
        r_port = _settings.getValue("PORT", 8000, 0);
        _settings.popTag();
    }
    
    // listen on the given port
    cout << "listening for osc messages on port " << r_port << endl;
    receiver.setup(r_port);
    // open an outgoing connection to HOST:PORT
    cout << "sending osc messages to host " << s_host << " using port " << r_port << endl;
    sender.setup(s_host, s_port);
    
    currentUpdate = &vhpOSC::receivePlayers;
}
void vhpOSC::send(int _players){
    cout << "send: " << _players << endl;
    ofxOscMessage m;
    m.setAddress("/players");
    m.addIntArg(_players);
    sender.sendMessage(m, false);
}

//--------------------------------------------------------------

void vhpOSC::stop(){
    
}

//--------------------------------------------------------------

void vhpOSC::start(){
    
}

//--------------------------------------------------------------
void vhpOSC::update(){
    (*this.*currentUpdate)();
}

void vhpOSC::receivePlayers(){
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        // check for mouse moved message
        if(m.getAddress() == "/players"){
            // argument is int32
            receivedPlayers = m.getArgAsInt32(0);
            cout << "players received! " << receivedPlayers << endl;
            int target = 3; // pending pass by parameter LEVELMENU           3
            ofNotifyEvent(playersReceived, target);
        }
    }
}

//--------------------------------------------------------------

ofEvent <int> vhpOSC::playersReceived;