#include "vhpPmThread.h"

//--------------------------------------------------------------
vhpPmThread::vhpPmThread(): video_start(0.0), video_end(1.0), count(0), goTo(0){

}

//--------------------------------------------------------------
void vhpPmThread::setup(ofVideoPlayer* _target, float _start, float _end){
    target = _target;
    video_start = _start;
    video_end = _end;
    currentUpdate = &vhpPmThread::loop;
}

//--------------------------------------------------------------
void vhpPmThread::reset(float _start, float _end){
    cout << "reset" << endl;
    if(lock()) {
        count = 0;
        video_start = _start;
        video_end = _end;
        currentUpdate = &vhpPmThread::loop;
        unlock();
    }
}

//--------------------------------------------------------------
void vhpPmThread::internalReset(float _start, float _end){
    cout << "internalReset" << endl;
    video_start = _start;
    video_end = _end;
    currentUpdate = &vhpPmThread::loop;
}

//--------------------------------------------------------------
void vhpPmThread::fadeReset(float _start, float _end, float _stored_start, float _stored_end){
    cout << "fadeReset" << endl;
    if(lock()) {
        count = 0;
        video_start = _start;
        video_end = _end;
        stored_start = _stored_start;
        stored_end = _stored_end;
        currentUpdate = &vhpPmThread::fadeLoop;
        unlock();
    }
}

//--------------------------------------------------------------
void vhpPmThread::stop(){
    stopThread();
}

//--------------------------------------------------------------
void vhpPmThread::start(){
    startThread(true, true);
}

//--------------------------------------------------------------
void vhpPmThread::update(){
    (*this.*currentUpdate)();
}

// Video Controller -------------------------------------------------
void vhpPmThread::loop(){
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
void vhpPmThread::fadeLoop(){
    // Do something
    cout << "video fadeLoop: " << target->getPosition() << endl;
    if (target->getPosition()>=video_end){
        target->setPosition(video_start);
        internalReset(stored_start, stored_end);
        cout << "thread is looping video and resetting loop" << endl;
    }
}

//--------------------------------------------------------------
void vhpPmThread::draw(){

}

/*
//--------------------------------------------------------------
int vhpPmThread::getCount() {
    unique_lock<std::mutex> lock(mutex);
    return count;
}
*/

//--------------------------------------------------------------
void vhpPmThread::threadedFunction(){
    while(isThreadRunning()) {
        if(lock()) {
            update();
            unlock();
            // Sleep for 1/2 second.
            sleep(500);
        }
    }
}

ofEvent <int> vhpPmThread::timeOut;