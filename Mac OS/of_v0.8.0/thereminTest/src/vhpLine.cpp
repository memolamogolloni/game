#include "vhpLine.h"

//--------------------------------------------------------------
vhpLine::vhpLine(){

}
vhpLine::vhpLine(string _line){
    cout << _line << endl;
    full = _line;
    setLine(_line);
    init();
}

//--------------------------------------------------------------
void vhpLine::setLine(string _line){
    texto = ofSplitString(_line, " ");
}

//--------------------------------------------------------------
void vhpLine::init(){
    visible = "";
    p = 0;
}

//--------------------------------------------------------------
void vhpLine::add(){
    visible += texto[p]+" ";
    p++;
}

//--------------------------------------------------------------
void vhpLine::draw(){
   
}

//--------------------------------------------------------------
bool vhpLine::isNotLast(){
    return p < texto.size();
}
string vhpLine::getVisibleLine(){
    return visible;
}