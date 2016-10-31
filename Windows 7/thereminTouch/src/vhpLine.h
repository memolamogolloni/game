#pragma once
#include "ofMain.h"

class vhpLine {

	public:
	
		// constructor
        vhpLine();
        vhpLine(string _line);
		
		// funcións ou métodos
		void setLine(string _line);
        void init();
        void add();
        void draw();
        string getVisibleLine();
    
        bool isNotLast();
		
		// Variables ou propiedades
        vector<string>              texto;
        int                         p;
        string                      visible;
        string                      full;
    
};
