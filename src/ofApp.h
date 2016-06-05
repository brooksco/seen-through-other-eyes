#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxGUI.h"
#include "ofxXmlSettings.h"
#include "spaceTime.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void generate(float lat, float lon, double time);
    
    string flickrUrl(float lat, float lon, double time);
    string mapsUrl(float lat, float lon);
    
    bool in_array(const string &value, const std::vector<string> &array);
    
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
    
    
    ofxJSONElement locationData;
    
    ofxJSONElement flickrResponse;
    ofxJSONElement mapsResponse;
    
    string localityText;
    string sublocalityText;
    
    vector<ofImage> images;
    vector<ofVec2f> imagePositions;
    string locationText;
    string timeText;
    
    spaceTime currentSpaceTime;
    spaceTime nextSpaceTime;
    
    
    // GUI
    ofxPanel gui;
    
    // GUI groups
    ofxGuiGroup miscGroup;
    
    // GUI toggles
    
    // GUI buttons;
    ofxButton nextButton;
    
    // XML settings
    ofxXmlSettings settings;
    
    
};

extern int offsetX;
extern int offsetY;
extern int maxImages;
extern int locationCount;
extern int totalLocations;
extern ofTrueTypeFont font;
