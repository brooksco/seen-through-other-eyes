//
//  spaceTime.cpp
//  photoLocation
//
//  Created by Colin Brooks on 6/4/16.
//
//

#include "ofApp.h"
#include "spaceTime.h"

void spaceTime::update() {
    
    if (opacities.size() > 0) {
    
    // Has one extra element in it for the text
    for (int i = opacities.size(); i >= 0; i--) {
        
        if (opacities[i] < 255) {
            
            if ((i - 1 >= 0)) {
                
                if (opacities[i - 1] >= 255) {
                    opacities[i] += 5;
                }
                
            } else {
                opacities[i] += 5;
            }
            
        } else {
            break;
        }
        
    }
    }
}

void spaceTime::draw() {
    offsetX = 0;
    offsetY = 0;
    
    if (images.size() > 0) {
        
        int count = MIN(maxImages, images.size());
        
        for (std::size_t i = 0; i < count; i++) {
            
            ofSetColor(255, 255, 255, opacities[i + 1]);
            
            images[i].draw(positions[i].x, positions[i].y);
            
            offsetX += images[i].getWidth();
            
            if (offsetX + images[i].getWidth() > ofGetWindowWidth()) {
                offsetX = 0;
                offsetY += images[i].getHeight();
            }
        }
    }
    
    ofSetColor(255, 255, 255, opacities[0]);
    font.drawString(time + "\n" + location, 20, ofGetWindowHeight() - 80);

}