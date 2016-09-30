#include "vhpThread.h"

//--------------------------------------------------------------
vhpThread::vhpThread(): video_start(0.0), video_end(1.0), count(0), goTo(0){

}

//--------------------------------------------------------------
void vhpThread::setup(ofVideoPlayer* _target, float _start, float _end){
    target = _target;
    video_start = _start;
    video_end = _end;
    currentUpdate = &vhpThread::loop;
}

//--------------------------------------------------------------
void vhpThread::reset(float _start, float _end){
    cout << "reset" << endl;
    if(lock()) {
        count = 0;
        video_start = _start;
        video_end = _end;
        currentUpdate = &vhpThread::loop;
        unlock();
    }
}

//--------------------------------------------------------------
void vhpThread::internalReset(float _start, float _end){
    cout << "internalReset" << endl;
    video_start = _start;
    video_end = _end;
    currentUpdate = &vhpThread::loop;
}

//--------------------------------------------------------------
void vhpThread::fadeReset(float _start, float _end, float _stored_start, float _stored_end){
    cout << "fadeReset" << endl;
    if(lock()) {
        count = 0;
        video_start = _start;
        video_end = _end;
        stored_start = _stored_start;
        stored_end = _stored_end;
        currentUpdate = &vhpThread::fadeLoop;
        unlock();
    }
}

//--------------------------------------------------------------
void vhpThread::stop(){
    stopThread();
}

//--------------------------------------------------------------
void vhpThread::start(){
    startThread(true, true);
}

//--------------------------------------------------------------
void vhpThread::update(){
    (*this.*currentUpdate)();
}

// Video Controller -------------------------------------------------
void vhpThread::loop(){
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
void vhpThread::fadeLoop(){
    // Do something
    cout << "video fadeLoop: " << target->getPosition() << endl;
    if (target->getPosition()>=video_end){
        target->setPosition(video_start);
        internalReset(stored_start, stored_end);
        cout << "thread is looping video and resetting loop" << endl;
    }
}

//--------------------------------------------------------------
void vhpThread::draw(){

}

/*
//--------------------------------------------------------------
int vhpThread::getCount() {
    unique_lock<std::mutex> lock(mutex);
    return count;
}
*/

//--------------------------------------------------------------
void vhpThread::threadedFunction(){
    while(isThreadRunning()) {
        if(lock()) {
            update();
            unlock();
            // Sleep for 1/2 second.
            sleep(500);
        }
    }
}

ofEvent <int> vhpThread::timeOut;