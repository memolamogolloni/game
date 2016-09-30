#include "ofMain.h"
#include "testApp.h"
#ifdef TARGET_OPENGLES
#include "ofGLProgrammableRenderer.h"
#endif

//========================================================================
int main( ){
	//ofSetLogLevel(OF_LOG_VERBOSE);
#ifdef TARGET_OPENGLES
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
#endif
	ofSetupOpenGL(1024,500, OF_WINDOW);		// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}