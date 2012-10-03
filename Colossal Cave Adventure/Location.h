//
//  Location.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-09-29.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Location__
#define __Colossal_Cave_Adventure__Location__

#include <iostream>
#include <string>
#include <vector>
#include "MotionVerb.h"

#include "Object.h"

using namespace std;

class Location {
    
private:
    int number;
    string shortDescription;
    string longDescription;
    
    vector<Object*> *objects;
    vector<bool> assets;
    vector<vector<MotionVerb*>*>* motionVerbs;
    
    vector<Location*> *accessibleLocations;
    
    void init(int number = -1, string longDescription = "", string shortDescription = "");
    
    const string listObjects();
    
    const int getAccessibleLocationIndex(Location* loc) const;
    
public:
    Location();
    Location(int number);
    Location(int number, string longDescription);
    Location(int number, string longDescription, string shortDescription);
    ~Location();
    
    const int getNumber() const;
    void setNumber(const int number);
    
    const string getShortDescription();
    void setShortDescription(const string desc);
    void appendToShortDescription(const string desc);
    
    const string getLongDescription();
    void setLongDescription(const string desc);
    void appendToLongDescription(const string desc);
    
    void addObject(Object* obj);
    
    void setAsset(const int index, const bool value = true);
    
    void addAccessibleLocation(Location* loc);
    void addMotionVerb(Location* loc, MotionVerb* verb);
    
    const string getAccessibleLocationsAndMotionVerbsAsString();

    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__Location__) */
