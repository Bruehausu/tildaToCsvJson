#pragma once

#include "ofMain.h"
#include "ofxCsv.h"
#include "ofxJSON.h"

using namespace wng;

typedef struct {
    string name;
    float latitude;
    float longitude;
} Hotel;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void processLine(string line);
    
        Boolean         displayMode;
        int             displayInterval;
    
        int             lineCount;
        vector <string> inputLines;
        vector <string> dataTypes;
        vector <Hotel>  hotels;
    
        ofxCsv gpiCsv;
        ofxCsv gdpCsv;
    
        ofxCsv csvRecorder;
		ofxJSONElement JSONRecorder;
};
