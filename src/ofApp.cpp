
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofNoFill();
    
    displayMode = true;
    displayInterval = 2;
    lineCount = 0;
    
    JSONRecorder["hotels"] = Json::arrayValue;
    
    cout << "beginning input" << endl;
    
    ofBuffer buffer;
    buffer = ofBufferFromFile("hotelsbase.txt");   !!!!!!!!!!!!!!!!!!!!!!!!!
    
    //Hotel new_hotel = {"test", 0.0, 0.0};
    //hotels.push_back( new_hotel );
    
    cout << "input in ofBuffer, size:" << buffer.size() << endl;
    
    if(buffer.size()) {
        
        cout << "buffer not empty" << endl;
        
        int i = 0;
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end();
             (it != end) && (i < 100000); ++it) {
            
            if ((i % 1000) == 0) {
                cout << "parsed " << i << " lines" << endl;
            }
            
            string line = *it;
            
            ofStringReplace(line, ",", "-");
            // print out the line
            //cout << line << endl;
            
            processLine(line);
            
            i += 1;
        }
        
        if (!displayMode){
            cout << "saving to output.csv" << endl;
            csvRecorder.saveFile( ofToDataPath( "output.csv" ));
        
            if (JSONRecorder.save("output.json", true)){
                cout << "JSON save sucsessful" << endl;
            } else {
                cout << "JSON save failed" << endl;
            }
            if (JSONRecorder.save("output-short.json", false)){
                cout << "collapsed JSON save sucsessful" << endl;
            } else {
                cout << "collapsed JSON save failed" << endl;
            }
        }
    }
    
}



//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    if (displayMode) {
        
        ofPushMatrix();
        
        //translate so that the center of the screen is 0,0
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofSetColor(255, 255, 255, 20);
        
        //draw a translucent wireframe sphere (ofNoFill() is on)
        ofPushMatrix();
        //add an extra spin at the rate of 1 degree per frame
        //ofRotate(ofGetFrameNum(), 0, 1, 0);
        ofRotate(mouseX, 0, 1, 0);
        ofDrawSphere(0, 0, 0, 300);
        ofPopMatrix();
        
        ofSetColor(255);
        for(unsigned int i = 0; i < hotels.size(); i++){
            
            //three rotations
            //two to represent the latitude and lontitude of the city
            //a third so that it spins along with the spinning sphere
            ofQuaternion latRot, longRot, spinQuat;
            latRot.makeRotate(hotels[i].latitude, 1, 0, 0);
            longRot.makeRotate(hotels[i].longitude, 0, 1, 0);
            spinQuat.makeRotate(mouseX, 0, 1, 0);
        
            //our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
            ofVec3f center = ofVec3f(0,0,300);
            //multiplying a quat with another quat combines their rotations into one quat
            //multiplying a quat to a vector applies the quat's rotation to that vector
            //so to to generate our point on the sphere, multiply all of our quaternions together then multiple the centery by the combined rotation
            ofVec3f worldPoint = latRot * longRot * spinQuat * center;
            
            //draw it and label it
            //ofDrawLine(ofVec3f(0,0,0), worldPoint);
            ofDrawCircle(worldPoint, 1);
            
            //set the bitmap text mode billboard so the points show up correctly in 3d
            //ofDrawBitmapString(hotels[i].name, worldPoint );
        }
        
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::processLine(string line){
    
    vector<string> parts = ofSplitString(line, "~", false, true);
    //printf ("number of elems in line: %lu\n", parts.size());
    
    string name;
    float latitude;
    float longitude;
    
    if (displayMode){
        if ( lineCount != 0){
            for(int i = 0; i < parts.size(); i++){
                if( i == 1) {
                    name = parts[i];
                } else if ( i == 12 ) {
                    latitude = ofToFloat(parts[i]);
                } else if ( i == 13 ) {
                    longitude = ofToFloat(parts[i]);
                }
            }
            
            Hotel new_hotel = {name, latitude, longitude};
            hotels.push_back( new_hotel );
        }
    } else {
        if (lineCount == 0) {
            //header case
        
            for(int i = 0; i < parts.size(); i++){
                csvRecorder.setString(lineCount, i, parts[i]);
            }
        
            dataTypes = parts;
        
        } else {
            //general case
        
            Json::Value elem;
            
            for(int i = 0; i < parts.size(); i++){
                if (i == 9) {
                    //cout << ofSplitString(parts[i], ",", false, true)[0] << endl;
                
                    csvRecorder.setString(lineCount, i, ofSplitString(parts[i], ",", false, true)[0]);
                } else {
                    csvRecorder.setString(lineCount, i, parts[i]);
                }
                elem[dataTypes[i]] = parts[i];
            }
            JSONRecorder["hotels"].append(elem);
        
        }
    }
    
    lineCount += 1;
}
