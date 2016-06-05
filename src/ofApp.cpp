#include "ofApp.h"

int offsetX = 0;
int offsetY = 0;
int maxImages = 10;
int locationCount = 0;
int totalLocations = 0;
ofTrueTypeFont font;

bool nextBool = false;
bool showGui = false;


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(24);
    
    localityText = "";
    sublocalityText = "";
    
    font.load("Lato-Regular.ttf", 30);
    

    // Initial test generation
//    generate(408106549, -739605501, 1464897070527);
    nextBool = true;
    
    // Setup GUI
    int guiWidth = 300;
    
    gui.setDefaultWidth(guiWidth);
    gui.setup();
    gui.setPosition(20, 20);
    
    // Misc group
    miscGroup.setup("Controls");
    miscGroup.add(nextButton.setup("Next"));
    
    gui.add(&miscGroup);
    miscGroup.setWidthElements(guiWidth);
    
    // Load previous settings
    gui.loadFromFile("settings.xml");
    
//    locationData.open("LocationHistory.json");
    
    if (!locationData.open("smallLocationHistory.json")) {
//    if (!locationData.open("LocationHistory.json")) {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
        cout << "Failure to open location data" << endl;
        
    } else {
         cout << "Loaded location data" << endl;
    }
    
    cout << "LOCATION DATA" << endl;
    cout << locationData["locations"].size() << endl;
    
    totalLocations = locationData["locations"].size();
    
//    for (int i = 0; i < locationData["locations"].size(); i += 1000) {
//        
//    }

    
}


void ofApp::generate(float lat, float lon, double time) {
    
    string fUrl = flickrUrl(lat, lon, time);
    string mUrl = mapsUrl(lat, lon);

    if (!mapsResponse.open(mUrl)) {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
        cout << "some maps failure" << endl;
    }
    
    cout << "MAPS RESPONSE" << endl;
    cout << mapsResponse << endl;
    
    // Maps response
    
    string sublocality = "";
    string locality = "";
    locationText = "";
    
    for (int i = 0; i < mapsResponse["results"].size(); i++) {
        
//        cout << mapsResponse["results"][i]["address_components"] << endl;
        
        for (int j = 0; j < mapsResponse["results"][i]["address_components"].size(); j++) {
            
            for (int k = 0; k < mapsResponse["results"][i]["address_components"][j]["types"].size(); k++) {
                
                string type = mapsResponse["results"][i]["address_components"][j]["types"][k].asString();
                
                if (type == "sublocality") {
                    sublocality = mapsResponse["results"][i]["address_components"][j]["long_name"].asString();
                    
                } else if (type == "locality") {
                    locality = mapsResponse["results"][i]["address_components"][j]["long_name"].asString();
                }
            }
            
        }
        
    }
    
    localityText = locality;
    sublocalityText = sublocality;
    
    if (sublocalityText != "") {
        locationText = sublocalityText + ", " + localityText + ".";
        
    } else {
        locationText = localityText + ".";
    }

    
    cout << "SUBLOCALITY" << endl;
    cout << sublocality << endl;
    
    
    cout << "LOCALITY" << endl;
    cout << locality << endl;
    
    
    // Flickr response
    
    if (!flickrResponse.open(fUrl)) {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
        cout << "some flickr failure" << endl;
    }
    
    std::size_t numImages = MIN(maxImages, flickrResponse["photos"]["photo"].size());
    
    cout << "FLICKR RESPONSE" << endl;
    cout << flickrResponse << endl;
    
    images.clear();
    imagePositions.clear();
    vector<float> opacities;
    // Push back an extra value for the text
    opacities.push_back(0);
    
    for (int i = 0; i < numImages; ++i) {
        
        int farm = flickrResponse["photos"]["photo"][i]["farm"].asInt();
        std::string id = flickrResponse["photos"]["photo"][i]["id"].asString();
        std::string secret = flickrResponse["photos"]["photo"][i]["secret"].asString();
        std::string server = flickrResponse["photos"]["photo"][i]["server"].asString();
        std::string url = "http://farm" + ofToString(farm) + ".static.flickr.com/" + server + "/" + id + "_" + secret + ".jpg";
        
        ofImage img;
        img.loadImage(url);
        images.push_back(img);
        
        // - 250 because images are max 500 x 500 it seems from flickr
        int rX = ofRandom(ofGetWindowWidth()) - 250;
        int rY = ofRandom(ofGetWindowHeight()) - 250;
        
        // Keep re-rolling for new coordinates if it looks like it's gonna overlap the text too much
        // - 600 because it's image height, 500, and a buffer for the text
        while ( (rX < (ofGetWindowWidth() / 2)) && (rY > (ofGetWindowHeight() - (600))) ) {
            rX = ofRandom(ofGetWindowWidth()) - 250;
            rY = ofRandom(ofGetWindowHeight()) - 250;
        }
        
        imagePositions.push_back(ofVec2f(rX, rY));
        
        opacities.push_back(0);
    }
    
    // Put together the time text
    time_t t = (time / 1000);
    struct tm *tm = localtime(&t);
    char buffer[40];
    
    strftime(buffer, 40, "%A, %B %d %G, %I:%M%p.", tm);
    timeText = buffer;
    
    // Store everything in the current spacetime
    currentSpaceTime = spaceTime();
    currentSpaceTime.location = locationText;
    currentSpaceTime.time = timeText;
    currentSpaceTime.images = images;
    currentSpaceTime.positions = imagePositions;
    currentSpaceTime.opacities = opacities;
}


//--------------------------------------------------------------
void ofApp::update(){
    
    currentSpaceTime.update();
    
    // Change on a timer, this is limited by the Google Maps API 2500 request a day maximum. So 1 every ~35 seconds
    if ((ofGetFrameNum() % (24 * 60)) == 0) {
        nextBool = true;
    }
    
    if (nextButton || nextBool) {
        nextBool = false;
        
        int nextLocation = ofRandom(totalLocations);
        
        cout << "NEXT LOCATION DATA" << endl;
        cout << locationData["locations"][nextLocation] << endl;
        
        float nextLat = locationData["locations"][nextLocation]["latitudeE7"].asFloat();
        float nextLon = locationData["locations"][nextLocation]["longitudeE7"].asFloat();
        double nextTime = stod(locationData["locations"][nextLocation]["timestampMs"].asString());
        
        
        generate(nextLat, nextLon, nextTime);
    }
    
}



//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    currentSpaceTime.draw();

    
    
    // Show the GUI if we want it
    if (showGui) {
        gui.draw();
    }
}

//--------------------------------------------------------------
string ofApp::flickrUrl(float lat, float lon, double time){
    
    string apiKey = "9449961c16abd291096e54bc4351afc2";
    stringstream stream;
    
    // Set the min and max times to half a day on either side of the timestamp, for now
    double minTime = (time / 1000) - 43200;
    double maxTime = (time / 1000) + 43200;
    
    stream << fixed << setprecision(0) << minTime;
    string sMinTime = stream.str();
    
    stream.str("");
    stream << fixed << setprecision(0) << maxTime;
    string sMaxTime = stream.str();
    
    
    // Convert to string and set precision (4 is max for Flickr api)
    stream.str("");
    stream << fixed << setprecision(0) << lat;
    string sLat = stream.str();
    
    stream.str("");
    stream << fixed << setprecision(0) << lon;
    string sLon = stream.str();
    
    // Add decimal points, since location data doesn't come with them
    sLat.insert((sLat.size() - 7), ".");
    sLon.insert((sLon.size() - 7), ".");

    
    // In KM
    string radius = "2";
    
    string url = "https://www.flickr.com/services/rest/?method=flickr.photos.search&api_key=" + apiKey + "&lat=" + sLat + "&lon=" + sLon + "&radius=" + radius + "&min_taken_date=" + sMinTime + "&max_taken_date=" + sMaxTime + "&format=json&nojsoncallback=1";
    
    cout << "FLICKR URL" << endl;
    cout << url << endl;
    
    return url;
}

//--------------------------------------------------------------
string ofApp::mapsUrl(float lat, float lon){

    string apiKey = "AIzaSyBfDv2ODxWsEUXh0uIbKFuOX7RcaqmsQF8";
    
    // Convert to string and set precision (4 is max for Flickr api)
    stringstream stream;
    stream << fixed << setprecision(0) << lat;
    string sLat = stream.str();
    
    stream.str("");
    stream << fixed << setprecision(0) << lon;
    string sLon = stream.str();
    
    // Add decimal points, since location data doesn't come with them
    sLat.insert((sLat.size() - 7), ".");
    sLon.insert((sLon.size() - 7), ".");
    
    string url = "https://maps.googleapis.com/maps/api/geocode/json?latlng=" + sLat + "," + sLon + "&key=" + apiKey;
    
    cout << "MAPS URL" << endl;
    cout << url << endl;
    
    return url;
}

//--------------------------------------------------------------
bool ofApp::in_array(const string &value, const std::vector<string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
    gui.saveToFile("settings.xml");
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_RETURN) {
        showGui = !showGui;
    }
    
    if (key == OF_KEY_RIGHT) {
        nextBool = true;
    }
    
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
