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

#include "Object.h"

using namespace std;

class Location {
    
private:
    int number;
    string shortDescription;
    string longDescription;
    
    vector<Object*> *objects;
    
    void init(int number = -1, string longDescription = "", string shortDescription = "");
    
    //void deallocObjects();
    const string listObjects();
    
public:
    Location();
    Location(int number);
    Location(int number, string longDescription);
    Location(int number, string longDescription, string shortDescription);
    ~Location();
    
    const int getNumber();
    void setNumber(const int number);
    
    const string getShortDescription();
    void setShortDescription(const string desc);
    void appendToShortDescription(const string desc);
    
    const string getLongDescription();
    void setLongDescription(const string desc);
    void appendToLongDescription(const string desc);
    
    void addObject(Object* obj);

    string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__Location__) */
