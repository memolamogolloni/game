#include "vhpStandbyThread.h"

//--------------------------------------------------------------
vhpStandbyThread::vhpStandbyThread(): video_start(0.0), video_end(1.0), count(0), goTo(0){

}

//--------------------------------------------------------------
void vhpStandbyThread::setup(ofVideoPlayer* _target, float _start, float _end){
    target = _target;
    video_start = _start;
    video_end = _end;
    currentUpdate = &vhpStandbyThread::loop;
}
void vhpStandbyThread::setOSC(ofxXmlSettings& _settings){
    
    // A–adir LOS PUERTOS Y EL HOST desde el documento xml de settings
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
    
}
void vhpStandbyThread::send(int _players){
    cout << "send: " << _players << endl;
    ofxOscMessage m;
    m.setAddress("/players");
    m.addIntArg(_players);
    sender.sendMessage(m, false);
}

//--------------------------------------------------------------
void vhpStandbyThread::reset(float _start, float _end){
    cout << "reset" << endl;
    if(lock()) {
        count = 0;
        video_start = _start;
        video_end = _end;
        currentUpdate = &vhpStandbyThread::loop;
        unlock();
    }
}

//--------------------------------------------------------------
void vhpStandbyThread::internalReset(float _start, float _end){
    cout << "internalReset" << endl;
    video_start = _start;
    video_end = _end;
    currentUpdate = &vhpStandbyThread::loop;
}

//--------------------------------------------------------------
void vhpStandbyThread::fadeReset(float _start, float _end, float _stored_start, float _stored_end){
    cout << "fadeReset" << endl;
    if(lock()) {
        count = 0;
        video_start = _start;
        video_end = _end;
        stored_start = _stored_start;
        stored_end = _stored_end;
        currentUpdate = &vhpStandbyThread::fadeLoop;
        unlock();
    }
}

//--------------------------------------------------------------
void vhpStandbyThread::stop(){
    stopThread();
}

//--------------------------------------------------------------
void vhpStandbyThread::start(){
    startThread(true, true);
}

//--------------------------------------------------------------
void vhpStandbyThread::update(){
    (*this.*currentUpdate)();
}

// Video Controller -------------------------------------------------
void vhpStandbyThread::loop(){
    // Do something
    cout << "video loop: " << target->getPosition() << endl;
    count ++;
    if (count>=20) {
        cout << "thread is going back to screensaver" << endl;
        count = 0;
        ofNotifyEvent(timeOut, goTo);
        
    } else if (target->getPosition()>=video_end){
        target->setPosition(video_start);
        cout << "thread is looping video" << endl;
    }
}

//--------------------------------------------------------------
void vhpStandbyThread::fadeLoop(){
    // Do something
    cout << "video fadeLoop: " << target->getPosition() << endl;
    if (target->getPosition()>=video_end){
        target->setPosition(video_start);
        internalReset(stored_start, stored_end);
        cout << "thread is looping video and resetting loop" << endl;
    }
}

//--------------------------------------------------------------
void vhpStandbyThread::draw(){

}

/*
//--------------------------------------------------------------
int vhpThread::getCount() {
    unique_lock<std::mutex> lock(mutex);
    return count;
}
*/

//--------------------------------------------------------------
void vhpStandbyThread::threadedFunction(){
    while(isThreadRunning()) {
        if(lock()) {
            update();
            unlock();
            // Sleep for 1/2 second.
            sleep(500);
        }
    }
}

ofEvent <int> vhpStandbyThread::timeOut;