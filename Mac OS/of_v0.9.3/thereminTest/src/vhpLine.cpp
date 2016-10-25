#include "vhpLine.h"

//--------------------------------------------------------------
vhpLine::vhpLine(){

}
vhpLine::vhpLine(string _line){
    cout << _line << endl;
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