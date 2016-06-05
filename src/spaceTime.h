//
//  spaceTime.h
//  photoLocation
//
//  Created by Colin Brooks on 6/4/16.
//
//

#ifndef spaceTime_h
#define spaceTime_h

struct spaceTime {
    
public:
    // Constructors
//    spaceTime();
    
    // Methods
    void draw();
    void update();
    
    // Variables
    vector<ofImage> images;
    vector<ofVec2f> positions;
    vector<float> opacities;
    
    string location;
    string time;
    
};


#endif /* spaceTime_h */
