#include "vhpGameCore.h"

// Constructor -------------------------------------------------

vhpGameCore::vhpGameCore(): scale(1.0),loaded(false),loading(false),angle(0){

    wHeight = 223;
    wY = 335;
    wWidth[0][0] = 172;
    wWidth[0][1] = 146;
    wWidth[0][2] = 138;
    wWidth[0][3] = 131;
    wWidth[0][4] = 125;
    wWidth[0][5] = 120;
    wWidth[0][6] = 128;
    wWidth[1][6] = 172;
    wWidth[1][5] = 146;
    wWidth[1][4] = 138;
    wWidth[1][3] = 131;
    wWidth[1][2] = 125;
    wWidth[1][1] = 120;
    wWidth[1][0] = 128;
    
    wX[0][0] = 0;
    for (int i = 1; i < nWINDOWS; i++) {
        wX[0][i] = wX[0][i-1] + wWidth[0][i-1];
    }
    wX[1][0] = wX[0][6] + wWidth[0][6];
    for (int i = 1; i < nWINDOWS; i++) {
        wX[1][i] = wX[1][i-1] + wWidth[1][i-1];
    }
    
}

vhpGameCore::~vhpGameCore(){
}

// Inicializar variables y cargar los archivos -----------------
void vhpGameCore::setup(vhpOSC* _mensajeria, int _currentScene, int _targetScene){
    
    mensajeria = _mensajeria;
    
    // FBO alocation and cleaning
    width = 1920;
    height = 1080;
    fbo.allocate(width, height, GL_RGBA);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();

    // Añadir las imágenes Sueltas
    loadingSilge.push_back(&building);
    filesSingle.push_back("g-fondo-edificio");
    loadingSilge.push_back(&score[0]);
    filesSingle.push_back("g-red-bar");
    loadingSilge.push_back(&score[1]);
    filesSingle.push_back("g-blue-bar");
    loadingSilge.push_back(&tie);
    filesSingle.push_back("g-empate");
    loadingSilge.push_back(&winnerBackground[0]);
    filesSingle.push_back("g-red-wins");
    loadingSilge.push_back(&winnerBackground[1]);
    filesSingle.push_back("g-blue-wins");
    loadingSilge.push_back(&winnerButton[0]);
    filesSingle.push_back("g-red-wins-button");
    loadingSilge.push_back(&winnerButton[1]);
    filesSingle.push_back("g-blue-wins-button");
    loadingSilge.push_back(&purple);
    filesSingle.push_back("g-purple");
    loadingSilge.push_back(&yellow);
    filesSingle.push_back("g-yellow");
    loadingSilge.push_back(&blue);
    filesSingle.push_back("g-blue");
    loadingSilge.push_back(&green);
    filesSingle.push_back("g-green");
    loadingSilge.push_back(&iconosR);
    filesSingle.push_back("g-iconos-rojo");
    loadingSilge.push_back(&iconosA);
    filesSingle.push_back("g-iconos-azul");
    loadingSilge.push_back(&marcos);
    filesSingle.push_back("g-marcos");
    loadingSilge.push_back(&avisos);
    filesSingle.push_back("g-avisos");
    loadingSilge.push_back(&click);
    filesSingle.push_back("g-click");
    loadingSilge.push_back(&bg);
    filesSingle.push_back("g-bg");
    loadingSilge.push_back(&balls);
    filesSingle.push_back("g-balls");
    loadingSilge.push_back(&shadow);
    filesSingle.push_back("g-shadow");
    loadingSilge.push_back(&wrong);
    filesSingle.push_back("g-wrong");
    loadingSilge.push_back(&keko);
    filesSingle.push_back("g-keko");
    loadingSilge.push_back(&glow);
    filesSingle.push_back("g-glow");
    loadingSilge.push_back(&shadowblue);
    filesSingle.push_back("g-shadow-blue");
    loadingSilge.push_back(&shadowred);
    filesSingle.push_back("g-shadow-red");
    loadingSilge.push_back(&buttonblue);
    filesSingle.push_back("g-button-blue");
    loadingSilge.push_back(&buttonred);
    filesSingle.push_back("g-button-red");
    loadingSilge.push_back(&colorBar[0]);
    filesSingle.push_back("g-red-b");
    loadingSilge.push_back(&colorBar[1]);
    filesSingle.push_back("g-blue-b");
    loadingSilge.push_back(&trofeo);
    filesSingle.push_back("g-trofeo");
    loadingSilge.push_back(&bandera);
    filesSingle.push_back("g-bandera");
    loadingSilge.push_back(&ventana);
    filesSingle.push_back("g-ventana");

    // Añadir las fuentes
    TTF.load("fonts/titilliumweblight.ttf", 22);
    TTF.setGlobalDpi(72);
    TTFB.load("fonts/titilliumweblight.ttf", 70);
    TTFB.setGlobalDpi(72);
    TTFM.load("fonts/titilliumweblight.ttf", 45);
    TTFM.setGlobalDpi(72);
    
    currentLoad = &vhpGameCore::loadSingle;
    
    // Inicializar las variables
    currentScene = _currentScene;   // SCREENSAVER
    targetScene = _targetScene;     // PLAYERMENU
    alpha = 0;
    alpha_increment = 5;
    count = 0;
    
    pWindow.setup();
    aWindowClick.setup();
    bWindowClick.setup();
    initGame();
    
}
void vhpGameCore::getText(string _file, bool _string) {
    if (_string) {
        ofBuffer buffer = ofBufferFromFile(_file);
        for (auto line : buffer.getLines()){
            cout << line << endl;
            lines.push_back(line);
        }
    } else {
        ofBuffer buffer = ofBufferFromFile(_file);
        for (auto line : buffer.getLines()){
            cout << line << endl;
            fLines.push_back(vhpLine(line));
        }
    }
}
void vhpGameCore::initGame(){
    getText("txt/g-text.txt", true);
    points[0] = 0;
    points[1] = 0;
    currentRound = 0;
    count = 0;
    for (int i=0; i<2; i++) {
        for (int u=0; u<7; u++) {
            windowState[i][u] = pendingW;
        }
    }
}
void vhpGameCore::initRound(){
    cout << "initRound()" << endl;
    isGo = false;
    holdSteady = ceil(ofRandom(3));
    targetsShot = randomWindow();
    mensajeria->send("targetsShot", targetsShot);
    alpha = 0;
    for (int i=0; i<4; i++) {
        alphaWindow[i] = 0;
    }
    alpha_increment = 5;
    soon[0] = false;
    soon[1] = false;
    clicked[0] = 7;
    clicked[1] = 7;
    ok[0] = false;
    ok[1] = false;
    hold[0] = false;
    hold[1] = false;
    next[0] = false;
    next[1] = false;
    winner = 0;
    delay = 90;
}
void vhpGameCore::initPattern(){
    currentWindow = 0;
    holdSteady = ceil(ofRandom(3));
    targetsShot = randomWindow();
    alpha = 0;
    for (int i=0; i<4; i++) {
        alphaWindow[i] = 0;
    }
    for (int i = 0; i<4; i++) {
        registeredPattern[0][i] = 7;
        registeredPattern[1][i] = 7;
    }
    alpha_increment = 5;
    soon[0] = false;
    soon[1] = false;
    clicked[0] = 7;
    clicked[1] = 7;
    ok[0] = false;
    ok[1] = false;
    hold[0] = false;
    hold[1] = false;
    next[0] = false;
    next[1] = false;
    winner = 0;
    delay = 90;
    for (int i = 0; i<nWINDOWS; i++) {
        windowState[0][i]= whiteW;
        windowState[1][i]= whiteW;
    }
}

// Precarga de todos los elementos -----------------------------
void vhpGameCore::load(){
    (*this.*currentLoad)();
}
void vhpGameCore::loadVideo(){
    
    /*
    if (!loaded) {
        int actual = videoFile.size() - 1;
        cout << "actual: " <<actual << " video.size(): " << video.size() << endl;
        if (actual>=0) {
            if(video[actual].isLoaded()) {
                cout << "video: " << videoFile[actual] << " is loaded!" << endl;
                videoFile.pop_back();
                loading = false;
            } else {
                if (!loading) {
                    video[actual].load(videoFile[actual]);
                    cout << "load video: " << videoFile[actual] << endl;
                    loading = true;
                }
            }
        } else {
            cout << "loading single videos finished!" << endl;
            width = video[0].getWidth();
            height = video[0].getHeight();
            cout << " video width: " << width << " height: " << height << endl;
            fbo.allocate(width, height, GL_RGBA);
            // clean FBO
            fbo.begin();
            ofClear(255,255,255, 0);
            fbo.end();
            loading = false;
            currentLoad = &vhpGameCore::loadSingle;
        }
    }
     */
}
void vhpGameCore::loadSingle(){
    if (!loaded) {
        int actual = filesSingle.size() -1;
        if (actual>=0) {
            if(loadingSilge[actual]->isAllocated()) {
                loadingSilge.pop_back();
                filesSingle.pop_back();
                loading = false;
            } else {
                if (!loading) {
                    loadingSilge[actual]->load("images/"+ filesSingle[actual] +".png");
                    cout << "images/"+ filesSingle[actual] +".png" << endl;
                    loading = true;
                }
            }
            
        } else {
            cout << "loading single images finished!" << endl;
            loaded = true;
        }
    }
}

// Comenzar e interrumpir los hilos y listeners de la escena ---
void vhpGameCore::start(){
    initGame();
}
void vhpGameCore::stop(){
}

// Dibujado y actualización variables --------------------------
void vhpGameCore::update(){
    (*this.*currentUpdate)();
    count++;
}
void vhpGameCore::updateTextLine(){
    // cout << count << endl;
    if (count%6==0){
        for (int i = 0; i < fLines.size(); i++) {
            if (fLines[i].isNotLast()) {
                fLines[i].add();
                break;
            }
        }
    }
}

void vhpGameCore::draw(int _x, int _y){
    fbo.draw(_x, _y, width, height);
}

// Dibujado de elementos ---------------------------------------
void vhpGameCore::drawScore(){
    ofPushStyle();
    int diff = points[0] - points[1];
    int widthA = 700 + (100*diff);
    int widthB = 700 - (100*diff);
    score[0].draw(260, 925, widthA, 81);
    score[1].draw(260 + widthA, 925, widthB, 81);
    ofSetColor(255,255,255);
    ofSetLineWidth(1);
    ofDrawLine(260 + widthA, 925+16, 260 + widthA, 925+16+49);
    ofPopStyle();
}
void vhpGameCore::drawGame(){
    ofSetColor(255, 255, 255);
    drawBackground();
    building.draw(0, 0);
    drawWindows();
    drawScore();
}
void vhpGameCore::drawWindows(){
    ofSetColor(255,255,255);
    for (int i = 0; i<nWINDOWS; i++) {
        switch (windowState[0][i]) {
            case pendingW:
                iconosR.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                marcos.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            case lostW:
                iconosA.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            case wonW:
                iconosR.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            case whiteW:
                avisos.drawSubsection(wX[0][i], wY, wWidth[0][i], wHeight, wX[0][i], wY, wWidth[0][i], wHeight);
                break;
            default:
                break;
        }
        switch (windowState[1][i]) {
            case pendingW:
                iconosA.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                marcos.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            case lostW:
                iconosR.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            case wonW:
                iconosA.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            case whiteW:
                avisos.drawSubsection(wX[1][i], wY, wWidth[1][i], wHeight, wX[1][i], wY, wWidth[1][i], wHeight);
                break;
            default:
                break;
        }
    }
}
void vhpGameCore::drawBackground(){
    bg.draw(0,0);
    shadow.draw(0,0);
    balls.draw(0,0);
}
void vhpGameCore::drawRound(){
    ofSetColor(255, 255, 255);
    ofRectangle box;
    box = TTFB.getStringBoundingBox(lines[0], 0, 0);
    ofRectangle center;
    center = TTFB.getStringBoundingBox(lines[0]+" "+ofToString(currentRound+1), 0, 0);
    TTFB.drawString(lines[0], (1920 - center.width)/2, 140);
    TTFB.drawString(ofToString(currentRound+1), (1920 - center.width)/2 + box.width + 10, 140);
    center = TTF.getStringBoundingBox(lines[1], 0, 0);
    TTF.drawString(lines[1], (1920 - center.width)/2, 210);
}
void vhpGameCore::drawPatternText(){
    ofSetColor(255, 255, 255);
    ofRectangle box;
    box = TTFB.getStringBoundingBox(lines[11], 0, 0);
    TTFB.drawString(lines[11], (1920 - box.width)/2, 140);
    box = TTF.getStringBoundingBox(lines[12], 0, 0);
    TTF.drawString(lines[12], (1920 - box.width)/2, 210);
}
void vhpGameCore::drawRoundWiner(){
    int x = 580;
    int y = 180;
    int margin = 0;
    ofRectangle box;
    ofSetColor(255, 255, 255);
    TTFB.drawString(lines[5], x, y);
    box = TTFB.getStringBoundingBox(lines[5], x, y);
    x += box.width + margin;
    if (winner==0) {
        ofSetColor(240, 27, 86);
        TTFB.drawString(lines[6], x, y);
        box = TTFB.getStringBoundingBox(lines[6], x, y);
        x += box.width + margin;
    } else {
        ofSetColor(24, 194, 201);
        TTFB.drawString(lines[7], x, y);
        box = TTFB.getStringBoundingBox(lines[7], x, y);
        x += box.width + margin;
    }
    ofSetColor(255, 255, 255);
    TTFB.drawString(lines[8], x, y);
    box = TTFB.getStringBoundingBox(lines[8], x, y);
    x += box.width + margin;
    TTFB.drawString(ofToString(currentRound), x, y);
    box = TTFB.getStringBoundingBox(ofToString(currentRound), x, y);
    x += box.width + margin;
    TTFB.drawString(lines[9], x, y);
}
void vhpGameCore::drawReadyButton(){
    ofRectangle box;
    box = TTFM.getStringBoundingBox("EE", 0, 0);
    int margin = box.width;
    ofSetRectMode(OF_RECTMODE_CORNER);
    box = TTFM.getStringBoundingBox(lines[10], 0, 0);
    colorBar[0].draw(340 - margin, 900, box.width + margin*2 + 12, TTFM.getLineHeight() + 12);
    TTFM.drawString(lines[10], 340, 900 + TTFM.getAscenderHeight());
    colorBar[1].draw(1580 - box.width - margin, 900, box.width + margin*2 + 12, TTFM.getLineHeight() + 12);
    TTFM.drawString(lines[10], 1580 - box.width, 900 + TTFM.getAscenderHeight());
}
void vhpGameCore::drawClickedWindow(){
    if (clicked[0]!=7) {
        ofSetColor(255,255,255,alphaWindow[0]);
        if (soon[0]==true) {
            wrong.drawSubsection(wX[0][clicked[0]], wY, wWidth[0][clicked[0]], wHeight, wX[0][clicked[0]], wY, wWidth[0][clicked[0]], wHeight);
        } else {
            click.drawSubsection(wX[0][clicked[0]], wY, wWidth[0][clicked[0]], wHeight, wX[0][clicked[0]], wY, wWidth[0][clicked[0]], wHeight);
        }
        alphaWindow[0] += 35;
        if (alphaWindow[0]>=255) alphaWindow[0] = 255;
    }
    if (clicked[1]!=7) {
        ofSetColor(255,255,255,alphaWindow[1]);
        if (soon[1]==true) {
            wrong.drawSubsection(wX[1][clicked[1]], wY, wWidth[1][clicked[1]], wHeight, wX[1][clicked[1]], wY, wWidth[1][clicked[1]], wHeight);
        } else {
            click.drawSubsection(wX[1][clicked[1]], wY, wWidth[1][clicked[1]], wHeight, wX[1][clicked[1]], wY, wWidth[1][clicked[1]], wHeight);
        }
        alphaWindow[1] += 35;
        if (alphaWindow[1]>=255) alphaWindow[1] = 255;
    }
}
void vhpGameCore::drawTextLine(int _x, int _y, int _alpha){
    ofSetColor(255, 255, 255, _alpha);
    for (int i = 0; i < fLines.size(); i++) {
        TTF.drawString(fLines[i].getVisibleLine(), _x, _y +(40*i));
    }
}

// Reproducir o detener la escena modificando currentUpdate ----
void vhpGameCore::play(){
    setRoundTutorial();
}
void vhpGameCore::pause(){
    currentUpdate = &vhpGameCore::pause;
}

// Procesado y actualización -----------------------------------

// Round Tutorial ----------------------------------------------
void vhpGameCore::setRoundTutorial(){
    initRound();
    fLines.clear();
    getText("txt/g-round-tutorial.txt", false);
    for (int i = 0; i < fLines.size(); i++) {
        fLines[i].init();
    }
    setTimeReference();
    currentUpdate = &vhpGameCore::showRoundTutorial;
    currentTouchPressed = &vhpGameCore::touchPressedRoundTutorial;
    /*  OSC  */
    /* ----- */
    mensajeria->send("gamestate", 0);
    /* ----- */
}
void vhpGameCore::showRoundTutorial(){
    
    // Update
    updateTextLine();
    if (getElapsedtime()>=6.0) {
        alpha += alpha_increment;
        if (alpha>=255) {
            alpha = 255;
        }
    }
    
    //Draw
    fbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    
    drawBackground();
    
    glPushMatrix();
    glTranslatef(0,100,0);
    building.draw(0, 0);
    drawWindows();
    ofSetColor(255,255,255,alpha);
    avisos.drawSubsection(wX[0][targetsShot], wY, wWidth[0][targetsShot], wHeight, wX[0][targetsShot], wY, wWidth[0][targetsShot], wHeight);
    avisos.drawSubsection(wX[1][targetsShot], wY, wWidth[1][targetsShot], wHeight, wX[1][targetsShot], wY, wWidth[1][targetsShot], wHeight);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0,-20,0);
    ofSetColor(255, 255, 255);
    keko.draw(0, 0);
    drawTextLine(425, 200, 255);
    ventana.draw(1435, 140);
    glPopMatrix();
    
    drawReadyButton();
    
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    
}

// Round -------------------------------------------------------
void vhpGameCore::setRound(){
    initRound();
    currentUpdate = &vhpGameCore::playReady;
    currentTouchPressed = &vhpGameCore::touchPressedGame;
    /*  OSC  */
    /* ----- */
    mensajeria->send("gamestate", 0);
    /* ----- */
}
void vhpGameCore::playReady(){
    // Reproduce el video y lo dibuja en el FBO
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,206,0,alpha);
    ofRectangle box = TTFB.getStringBoundingBox(lines[2], 0, 0);
    TTFB.drawString(lines[2], (width-box.width)/2, 840);
    drawClickedWindow();
    drawRound();
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>255) {
        alpha = 255;
        alpha_increment = -1 * alpha_increment;
    } else if (alpha<0) {
        alpha = 0;
        alpha_increment = -1 * alpha_increment;
        currentUpdate = &vhpGameCore::playSteady;
        mensajeria->send("gamestate", 1);
    }
}
void vhpGameCore::playSteady(){
    // Reproduce el video y lo dibuja en el FBO
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    ofSetColor(255,206,0,alpha);
    ofRectangle box = TTFB.getStringBoundingBox(lines[3], 0, 0);
    TTFB.drawString(lines[3], (width-box.width)/2, 840);
    drawClickedWindow();
    drawRound();
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        alpha_increment = -1 * alpha_increment;
    } else if (alpha<=0) {
        alpha = 0;
        alpha_increment = -1 * alpha_increment;
        if (holdSteady<=0) {
            currentUpdate = &vhpGameCore::playGo;
            mensajeria->send("gamestate", 2);
        }
        holdSteady --;
    }
}
void vhpGameCore::playGo(){
    // Reproduce el video y lo dibuja en el FBO
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    ofSetColor(255,206,0,alpha);
    ofRectangle box = TTFB.getStringBoundingBox(lines[4], 0, 0);
    TTFB.drawString(lines[4], (width-box.width)/2, 840);
    drawClickedWindow();
    drawRound();
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
        alpha_increment = -1 * alpha_increment;
    } else if (alpha<=0) {
        alpha = 0;
        alpha_increment = -1 * alpha_increment;
        isGo = true;
        setTimeReference();
        currentUpdate = &vhpGameCore::showWindow;
        /*  OSC  */
        /* ----- */
        cout << "sending window/round is " << targetsShot << endl;
        mensajeria->send("window/round", targetsShot);
        /* ----- */
    }
}
void vhpGameCore::showWindow(){
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    avisos.drawSubsection(wX[0][targetsShot], wY, wWidth[0][targetsShot], wHeight, wX[0][targetsShot], wY, wWidth[0][targetsShot], wHeight);
    avisos.drawSubsection(wX[1][targetsShot], wY, wWidth[1][targetsShot], wHeight, wX[1][targetsShot], wY, wWidth[1][targetsShot], wHeight);
    drawClickedWindow();
    drawRound();
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    if (getElapsedtime()>=3.0) {
        alpha += alpha_increment;
        if (alpha>=255) alpha = 255;
        checkRoundWinner();
    }
}

// Show Winner -------------------------------------------------
void vhpGameCore::showWinner(){
    // Reproduce el video y lo dibuja en el FBO
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,alpha);
    winnerBackground[winner].draw(0,0);
    ofSetColor(255, 255, 255);
    drawRoundWiner();
    winnerButton[0].draw(0,0);
    winnerButton[1].draw(0,0);
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) alpha = 255;
}

// Show Tie ----------------------------------------------------
void vhpGameCore::showTie(){
    // Reproduce el video y lo dibuja en el FBO
    fbo.begin();
    drawGame();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    tie.draw(0,0);
    winnerButton[0].draw(0,0);
    winnerButton[1].draw(0,0);
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) alpha = 255;
}

// Pattern Tutorial --------------------------------------------
void vhpGameCore::setPatternTutorial(){
    currentUpdate = &vhpGameCore::showPatternTutorial;
    currentTouchPressed = &vhpGameCore::touchPressedPatternTutorial;
    mensajeria->send("gamestate", 4);
    fLines.clear();
    getText("txt/g-pattern-tutorial.txt", false);
    for (int i = 0; i < fLines.size(); i++) {
        fLines[i].init();
    }
    initPattern();
    randomPattern();
    pWindow.setWindows(&purple, targetsPattern[0], &yellow, targetsPattern[1], &blue, targetsPattern[2], &green, targetsPattern[3]);
    pWindow.setFadeIn();
    setTimeReference();
}
void vhpGameCore::showPatternTutorial(){
    
    // Update
    updateTextLine();
    if (getElapsedtime()>8.0) pWindow.update();
    
    //Draw
    fbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    
    drawBackground();
    
    glPushMatrix();
    glTranslatef(0,100,0);
    building.draw(0, 0);
    drawWindows();
    if (getElapsedtime()>8.0) pWindow.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-20,0);
    ofSetColor(255, 255, 255);
    keko.draw(0, 0);
    drawTextLine(425, 200, 255);
    bandera.draw(1435, 140);
    glPopMatrix();
    
    drawReadyButton();
    
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    
}

// Show Window Pattern -----------------------------------------
void vhpGameCore::setWindowPattern(){
    initPattern();
    randomPattern();
    setTimeReference(3.0);
    pWindow.setWindows(&purple, targetsPattern[0], &yellow, targetsPattern[1], &blue, targetsPattern[2], &green, targetsPattern[3]);
    aWindowClick.setWindows(&purple, targetsPattern[0], &yellow, targetsPattern[1], &blue, targetsPattern[2], &green, targetsPattern[3]);
    bWindowClick.setWindows(&purple, targetsPattern[0], &yellow, targetsPattern[1], &blue, targetsPattern[2], &green, targetsPattern[3]);
    pWindow.order = 0;
    pWindow.setFadeIn();
    aWindowClick.setHiddenOne();
    bWindowClick.setHiddenOne();
    currentUpdate = &vhpGameCore::sendWindowPattern;
    currentTouchPressed = &vhpGameCore::touchPressedPattern;
}
void vhpGameCore::sendWindowPattern(){
    
    // Update
    aWindowClick.update();
    bWindowClick.update();
    // send patern using OSC
    if (getElapsedtime()>=3.0) {
        if (currentWindow<4) {
            /*  OSC  */
            /* ----- */
            cout << "sending window/patern " << currentWindow << " is " << targetsPattern[currentWindow] << endl;
            mensajeria->send("window/patern", targetsPattern[currentWindow]);
            /* ----- */
            currentWindow++;
            setTimeReference();
        } else if (currentWindow==4) {
            currentUpdate = &vhpGameCore::showPattern;
        }
    }
    
    // Draw
    fbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    drawGame();
    drawPatternText();
    aWindowClick.draw();
    bWindowClick.draw();
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    
}
void vhpGameCore::showPattern(){
    
    // Update
    pWindow.update();
    aWindowClick.update();
    bWindowClick.update();
    
    // Draw
    fbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    drawGame();
    drawPatternText();
    pWindow.draw();
    aWindowClick.draw();
    bWindowClick.draw();
    
    checkPatternWinner();
    
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    
}

// Show Winner -------------------------------------------------
void vhpGameCore::setFinalWinner(){
    currentUpdate = &vhpGameCore::showFinalWinner;
    currentTouchPressed = &vhpGameCore::touchPressedPatternWinner;
    mensajeria->send("gamestate", 4);
    fLines.clear();
    getText("txt/g-final.txt", false);
    for (int i = 0; i < fLines.size(); i++) {
        fLines[i].init();
    }
}
void vhpGameCore::showFinalWinner(){
    // Reproduce el video y lo dibuja en el FBO
    //video[0].update();
    
    updateTextLine();
    
    fbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255);
    bg.draw(0, 0);
    keko.draw(0, 0);
    if (winner==0) {
        shadowred.draw(0,0);
        buttonred.draw(75,915);
    } else {
        shadowblue.draw(0,0);
        buttonblue.draw(1575,915);
    }
    ofSetColor(255,255,255);
    glPushMatrix();
    glTranslatef(width/2,140+height/2,0);
    glRotatef(angle,0,1,0);
    trofeo.draw(0,0);
    trofeo.setAnchorPercent(0.5f, 0.5f);
    glPopMatrix();
    angle++;
    if (angle == 360) {
        angle = 0;
    }
    //trofeo.draw(0,0);
    //video[0].draw(0, 0);
    ofSetColor(255,255,255,alpha);
    //winnerBackground[winner].draw(0,0);
    ofSetColor(255, 255, 255);
    //drawRoundWiner();
    drawTextLine(425, 200, 255);
    
    
    ofDisableAlphaBlending();
    ofPopStyle();
    fbo.end();
    alpha += alpha_increment;
    if (alpha>=255) {
        alpha = 255;
    }
}

// Eventos -----------------------------------------------------
void vhpGameCore::touchPressed(float & _x, float & _y){
    (*this.*currentTouchPressed)(_x, _y);
}
void vhpGameCore::touchPressedRoundTutorial(float & _x, float & _y){
    
    cout << "MousePressed during round tutorial!" << endl;
    // min y: 870, max y: 870 + 60 + 88
    
    cout << "mouse x: " << _y << " mouse y: " << _x << endl;
    if ((_y>=840)&&(_y<=1020)) {
        // Player A
        if ((_x>=240)&(_x<=650)) {
            cout << "Is player A" << endl;
            if (!hold[0]) {
                cout << "Set next[0] true" << endl;
                next[0] = true;
                hold[0] = true;
                if (next[1]==true) setRound();
            }
            // Player B
        } else if ((_x>=1270)&(_x<=1680)) {
            cout << "Is player B" << endl;
            if (!hold[1]) {
                cout << "Set next[1] true" << endl;
                next[1] = true;
                hold[1] = true;
                if (next[0]==true) setRound();
            }
            
        }
    }
}
void vhpGameCore::touchPressedGame(float & _x, float & _y){
    cout << "MousePressed in game!" << endl;
    //ofNotifyEvent(onClick, gameTarget);
    
    // min y: 353, max y: 540
    cout << "mouse x: " << _y*3/scale << " mouse y: " << _x*3/scale << endl;
    if ((_y>=350)&&(_y<=545)) {
        // Player A
        if (_x<=960) {
            if (!hold[0]) {
                hold[0] = true;
                time[0] = ofGetElapsedTimeMillis();
                // Window 1
                if (_x<=171) {
                    clicked[0] = 0;
                    mensajeria->send("windowclick/a", 0);
                    cout << "Window 0" << endl;
                    
                    // Window 2
                } else if (_x<=316) {
                    clicked[0] = 1;
                    mensajeria->send("windowclick/a", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 3
                } else if (_x<=455) {
                    clicked[0] = 2;
                    mensajeria->send("windowclick/a", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 4
                } else if (_x<=587) {
                    clicked[0] = 3;
                    mensajeria->send("windowclick/a", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 5
                } else if (_x<=711) {
                    clicked[0] = 4;
                    mensajeria->send("windowclick/a", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 6
                } else if (_x<=832) {
                    clicked[0] = 5;
                    mensajeria->send("windowclick/a", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 7
                } else {
                    clicked[0] = 6;
                    mensajeria->send("windowclick/a", 6);
                    cout << "Window 6" << endl;
                    
                }
                if (clicked[0]==targetsShot) ok[0] = true;
                checkIsGo(0);
                cout << "TargetShot " << targetsShot << endl;
            }
        // Player B
        } else {
            if (!hold[1]) {
                hold[1] = true;
                time[1] = ofGetElapsedTimeMillis();
                // Window 7
                if (_x<=1088) {
                    clicked[1] = 0;
                    mensajeria->send("windowclick/b", 0);
                    cout << "Window 0" << endl;
                    
                    // Window 6
                } else if (_x<=1210) {
                    clicked[1] = 1;
                    mensajeria->send("windowclick/b", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 5
                } else if (_x<=1334) {
                    clicked[1] = 2;
                    mensajeria->send("windowclick/b", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 4
                } else if (_x<=1466) {
                    clicked[1] = 3;
                    mensajeria->send("windowclick/b", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 3
                } else if (_x<=1603) {
                    clicked[1] = 4;
                    mensajeria->send("windowclick/b", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 2
                } else if (_x<=1748) {
                    clicked[1] = 5;
                    mensajeria->send("windowclick/b", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 1
                } else {
                    clicked[1] = 6;
                    mensajeria->send("windowclick/b", 6);
                    cout << "Window 6" << endl;
                    
                }
                if (clicked[1]==targetsShot) ok[1] = true;
                checkIsGo(1);
                cout << "TargetShot " << targetsShot << endl;
            }
        }
    }
}
void vhpGameCore::touchPressedWinner(float & _x, float & _y){
    
    cout << "MousePressed after game!" << endl;
    
    // min y: 690, max y: 860
    cout << "mouse x: " << _y << " mouse y: " << _x << endl;
    if ((_y>=690)&&(_y<=860)) {
        // Player A
        if (_x<=960) {
            cout << "Is player A" << endl;
            if (!hold[0]) {
                cout << "Set next[0] true" << endl;
                next[0] = true;
                hold[0] = true;
                if (next[1]==true) {
                    cout << "next[1] is true" << endl;
                    currentRound++;
                    // cuatro primeras rondas
                    if (currentRound<nROUNDS) {
                        cout << "Next Round!" << endl;
                        initRound();
                        currentUpdate = &vhpGameCore::playReady;
                        currentTouchPressed = &vhpGameCore::touchPressedGame;
                        // última ronda
                    } else {
                        cout << "Last Round!" << endl;
                        setPatternTutorial();
                    }
                }
            }
        // Player B
        } else {
            cout << "Is player B" << endl;
            if (!hold[1]) {
                cout << "Set next[1] true" << endl;
                next[1] = true;
                hold[1] = true;
                if (next[0]==true) {
                    cout << "next[0] is true" << endl;
                    currentRound++;
                    // cuatro primeras rondas
                    if (currentRound<nROUNDS) {
                        cout << "Next Round!" << endl;
                        initRound();
                        currentUpdate = &vhpGameCore::playReady;
                        currentTouchPressed = &vhpGameCore::touchPressedGame;
                        // última ronda
                    } else {
                        cout << "Last Round!" << endl;
                        setPatternTutorial();
                    }
                }
            }
        }
    }
}
void vhpGameCore::touchPressedPatternTutorial(float & _x, float & _y){
    
    cout << "MousePressed during tutorial!" << endl;
    // min y: 870, max y: 870 + 60 + 88
    
    cout << "mouse x: " << _y << " mouse y: " << _x << endl;
    if ((_y>=840)&&(_y<=1020)) {
        // Player A
        if ((_x>=240)&(_x<=650)) {
            cout << "Is player A" << endl;
            if (!hold[0]) {
                cout << "Set next[0] true" << endl;
                next[0] = true;
                hold[0] = true;
                if (next[1]==true) setWindowPattern();
            }
        // Player B
        } else if ((_x>=1270)&(_x<=1680)) {
            cout << "Is player B" << endl;
            if (!hold[1]) {
                cout << "Set next[1] true" << endl;
                next[1] = true;
                hold[1] = true;
                if (next[0]==true) setWindowPattern();
            }

        }
    }
}
void vhpGameCore::touchPressedPattern(float & _x, float & _y){
    cout << "MousePressed during pattern!" << endl;
    int n[2] ={0,0};
    for (int i = 0; i<2; i++) {
        if(registeredPattern[i][2]!=7) {
            n[i] = 3;
        } else if(registeredPattern[i][1]!=7) {
            n[i] = 2;
        } else if(registeredPattern[i][0]!=7) {
            n[i] = 1;
        }
    }
    
    // min y: 353, max y: 540
    cout << "click: " << n[0] << " mouse x: " << _y*3/scale << " mouse y: " << _x*3/scale << endl;
    
    if ((_y>=350)&&(_y<=545)) {
        // Player A
        if (_x<=960) {
            if (!isGo) {
                hold[0] = true;
                ok[0] = false;
            } else if (!hold[0]) {
                // Window 1
                if (_x<=171) {
                    registeredPattern[0][n[0]] = 0;
                    mensajeria->send("windowclick/a", 0);
                    cout << "Window 0" << endl;
                    
                    // Window 2
                } else if (_x<=316) {
                    registeredPattern[0][n[0]] = 1;
                    mensajeria->send("windowclick/a", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 3
                } else if (_x<=455) {
                    registeredPattern[0][n[0]] = 2;
                    mensajeria->send("windowclick/a", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 4
                } else if (_x<=587) {
                    registeredPattern[0][n[0]] = 3;
                    mensajeria->send("windowclick/a", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 5
                } else if (_x<=711) {
                    registeredPattern[0][n[0]] = 4;
                    mensajeria->send("windowclick/a", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 6
                } else if (_x<=832) {
                    registeredPattern[0][n[0]] = 5;
                    mensajeria->send("windowclick/a", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 7
                } else {
                    registeredPattern[0][n[0]] = 6;
                    mensajeria->send("windowclick/a", 6);
                    cout << "Window 6" << endl;
                }
                aWindowClick.setOneWindow(n[0], registeredPattern[0][n[0]], 0);
                if ((n[0]==3)&&(registeredPattern[0][3]!=7)) {
                    cout << "Lets check" << endl;
                    time[0] = ofGetElapsedTimeMillis();
                    hold[0] = true;
                    if ((targetsPattern[0]==registeredPattern[0][0])&&(targetsPattern[1]==registeredPattern[0][1])&&(targetsPattern[2]==registeredPattern[0][2])&&(targetsPattern[3]==registeredPattern[0][3])) {
                        ok[0] = true;
                        cout << "Player 0 has succeded!" << endl;
                    }
                }
            }
        // Player B
        } else {
            if (!isGo) {
                hold[1] = true;
                ok[1] = false;
            } if (!hold[1]) {
                // Window 7
                if (_x<=1088) {
                    registeredPattern[1][n[1]] = 6;
                    mensajeria->send("windowclick/b", 6);
                    cout << "Window 6" << endl;
                    
                    // Window 6
                } else if (_x<=1210) {
                    registeredPattern[1][n[1]] = 5;
                    mensajeria->send("windowclick/b", 5);
                    cout << "Window 5" << endl;
                    
                    // Window 5
                } else if (_x<=1334) {
                    registeredPattern[1][n[1]] = 4;
                    mensajeria->send("windowclick/b", 4);
                    cout << "Window 4" << endl;
                    
                    // Window 4
                } else if (_x<=1466) {
                    registeredPattern[1][n[1]] = 3;
                    mensajeria->send("windowclick/b", 3);
                    cout << "Window 3" << endl;
                    
                    // Window 3
                } else if (_x<=1603) {
                    registeredPattern[1][n[1]] = 2;
                    mensajeria->send("windowclick/b", 2);
                    cout << "Window 2" << endl;
                    
                    // Window 2
                } else if (_x<=1748) {
                    registeredPattern[1][n[1]] = 1;
                    mensajeria->send("windowclick/b", 1);
                    cout << "Window 1" << endl;
                    
                    // Window 1
                } else {
                    registeredPattern[1][n[1]] = 0;
                    mensajeria->send("windowclick/b", 0);
                    cout << "Window 0" << endl;
                    
                }
                // hay discordancias entre el orden de las ventanas, si es igual o en espejo corregir para arreglar este apaño
                aWindowClick.setOneWindow(n[1], 6-registeredPattern[1][n[1]], 1);
                if ((n[1]==3)&&(registeredPattern[1][3]!=7)) {
                    cout << "Lets check" << endl;
                    time[1] = ofGetElapsedTimeMillis();
                    hold[1] = true;
                    if ((targetsPattern[0]==registeredPattern[1][0])&&(targetsPattern[1]==registeredPattern[1][1])&&(targetsPattern[2]==registeredPattern[1][2])&&(targetsPattern[3]==registeredPattern[1][3])) {
                        ok[1] = true;
                        cout << "Player 1 has succeded!" << endl;
                    }
                }
                
            }
        }
    }
}
void vhpGameCore::touchPressedPatternWinner(float & _x, float & _y){
    //
    cout << "MousePressed after Pattern!" << endl;
    //
    
    // min y: 690, max y: 860
    cout << "mouse x: " << _y << " mouse y: " << _x << endl;
    // Player A
    if ((winner==0)&&(_y>=894)&&(_y<=1036)&&(_x>=10)&&(_x<=410)) {
        ofNotifyEvent(onRestart, targetScene);
    // Player B
    } else if ((winner==1)&&(_y>=894)&&(_y<=1036)&&(_x>=1510)&&(_x<=1910)) {
        ofNotifyEvent(onRestart, targetScene);
    }
}

void vhpGameCore::checkIsGo(int _p){
    if (!isGo) {
        cout << "Player " << _p << " clicked to soon!" << endl;
        hold[_p] = true;
        ok[_p] = false;
        soon[_p] = true;
    }
}
void vhpGameCore::checkPatternWinner(){
    if (hold[0]&&hold[1]) {
        delay--;
        if (delay<=0) {
            alpha = 0;
            bool tie = false;
            if (ok[0]&&ok[1]) {
                cout << "Both user have trigered a correct pattern" << endl;
                cout << "Time A: " << time[0] << endl;
                cout << "Time B: " << time[1] << endl;
                // foron igual de rápidos
                if (time[0]==time[1]) {
                    cout << "Both players where right!" << endl;
                    tie = true;
                } if (time[0]<time[1]) {
                    cout << "Player 0 was faster!" << endl;
                    winner = 0;
                } else {
                    cout << "Player 1 was faster!" << endl;
                    winner = 1;
                }
            } else {
                // acertou só 0
                if (ok[0]) {
                    cout << "Only 0 was succesful" << endl;
                    winner = 0;
                // acertou só 1
                } else if (ok[1]) {
                    cout << "Only 1 was succesful" << endl;
                    winner = 1;
                // no acertou ninguén
                } else {
                    cout << "Both players failed" << endl;
                    tie = true;
                }
            }
            if (!tie) points[winner] += 2;
            (points[0]>points[1]) ? winner = 0 : winner = 1;
            setFinalWinner();
        }
    }
}
void vhpGameCore::checkRoundWinner(){
    if ((alphaWindow[0]==255)&&(alphaWindow[1]==255)) {
        delay--;
        if (delay<=0) {
            cout << "Both user have trigered a window" << endl;
            cout << "Time A: " << time[0] << endl;
            cout << "Time B: " << time[1] << endl;
            alpha = 0;
            bool tie = false;
            // os dous acertaron
            if (ok[0]&&ok[1]) {
                // gaña o mais rápido
                if (time[0]<=time[1]) {
                    cout << "Player 0 was faster!" << endl;
                    winner = 0;
                    windowState[0][targetsShot] = wonW;
                    windowState[1][targetsShot] = lostW;
                } else {
                    cout << "Player 1 was faster!" << endl;
                    winner = 1;
                    windowState[0][targetsShot] = lostW;
                    windowState[1][targetsShot] = wonW;
                }
            } else {
                // acertou só 0
                if (ok[0]) {
                    cout << "Only 0 was succesful" << endl;
                    winner = 0;
                    windowState[0][targetsShot] = wonW;
                    windowState[1][targetsShot] = lostW;
                    // acertou só 1
                } else if (ok[1]) {
                    cout << "Only 1 was succesful" << endl;
                    winner = 1;
                    windowState[0][targetsShot] = lostW;
                    windowState[1][targetsShot] = wonW;
                    // no acertou ninguén
                } else {
                    cout << "Both players failed" << endl;
                    tie = true;
                }
            }
            if (tie) {
                hold[0] = false;
                hold[1] = false;
                currentRound --;
                currentUpdate = &vhpGameCore::showTie;
                currentTouchPressed = &vhpGameCore::touchPressedWinner;
                mensajeria->send("gamestate", 5);
            } else {
                hold[0] = false;
                hold[1] = false;
                points[winner]++;
                currentUpdate = &vhpGameCore::showWinner;
                currentTouchPressed = &vhpGameCore::touchPressedWinner;
                mensajeria->send("gamestate", 4);
            }
            
        }
    }
}

int vhpGameCore::randomWindow(){
    if (nums.size()==0) {
        for (int i=0; i<nWINDOWS; i++) {
            if (windowState[0][i]==pendingW) {
                nums.push_back(i);
            }
        }
    }
    return nums[ceil(ofRandom(nums.size()-1))];
}
void vhpGameCore::randomPattern(){
    int first = ceil(ofRandom(nWINDOWS-1));
    cout << "firt =" << first << endl;
    vector<int> nums;
    for (int i=0; i<nWINDOWS; i++) {
        if (i==first) {
            targetsPattern[0]= i;
            cout << "targetsPattern[0]=" << i << endl;
        } else {
            nums.push_back(i);
        }
    }
    int second = ceil(ofRandom(nums.size()-1));
    cout << "second =" << second << endl;
    vector<int> snums;
    for (int i=0; i<nums.size(); i++) {
        if (i==second) {
            targetsPattern[1]= nums[i];
            cout << "targetsPattern[1]=" << nums[i] << endl;
        } else {
            snums.push_back(nums[i]);
        }
    }
    int third = ceil(ofRandom(snums.size()-1));
    cout << "third =" << third << endl;
    vector<int> tnums;
    for (int i=0; i<snums.size(); i++) {
        if (i==third) {
            targetsPattern[2]= snums[i];
            cout << "targetsPattern[2]=" << snums[i] << endl;
        } else {
            tnums.push_back(snums[i]);
        }
    }
    
    int fourth = ceil(ofRandom(tnums.size()-1));
    cout << "fourth =" << fourth << endl;
    for (int i=0; i<tnums.size(); i++) {
        if (i==fourth) {
            targetsPattern[3]= tnums[i];
            cout << "targetsPattern[3]=" << tnums[i] << endl;
        }
    }
}

void vhpGameCore::setTimeReference(){
    tRef = ofGetElapsedTimef();
}
void vhpGameCore::setTimeReference(float _ago){
    tRef = ofGetElapsedTimef();
    tRef -= _ago;
}

float vhpGameCore::getElapsedtime(){
    return ofGetElapsedTimef() - tRef;
}


ofEvent <int> vhpGameCore::onClick;
ofEvent <int> vhpGameCore::onRestart;