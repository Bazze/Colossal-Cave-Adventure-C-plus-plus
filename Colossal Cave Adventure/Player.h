//
//  Player.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-10-05.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Player__
#define __Colossal_Cave_Adventure__Player__

#include <iostream>
#include <vector>
#include "Object.h"
#include "Location.h"

class Player {
  
private:
    int score;
    Location* currentLocation;
    
    vector<Object*> *objects;
    
public:
    Player();
    virtual ~Player();

    int getScore() const;
    void setScore(int s);
    void addScore(int s);
    
    void setCurrentLocation(Location* loc);
    Location* getCurrentLocation() const;
    
    bool hasObject(Object* obj) const;
    vector<Object*>* getObjects() const;

    const string toString();

};

#endif /* defined(__Colossal_Cave_Adventure__Player__) */
