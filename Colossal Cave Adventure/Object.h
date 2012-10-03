//
//  Object.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-09-29.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Object__
#define __Colossal_Cave_Adventure__Object__

#include <iostream>
#include <string>
#include <vector>

#include "Word.h"

class Object : public Word {
    
private:
    int propertyValue;
    string inventoryMessage;
    vector<string> *descriptions;
    
    bool treasure;
    bool movable;
    
public:
    Object(int number, string word, string comment = "");
    ~Object();
    
    const int getPropertyValue();
    void setPropertyValue(const int propertyValue);
    
    const string getInventoryMessage();
    void setInventoryMessage(const string msg);
    
    void setDescriptionsVector(vector<string> *vector);
    const string getDescriptionForPropertyValue(const int n);
    const string getPropertyDescriptions();
    void addPropertyDescription(const string desc);
    void appendToPropertyDescription(const int n, const string desc);
    
    const bool isTreasure();
    void setTreasure(const bool treasure);
    
    const bool isMovable();
    void setMovable(const bool movable);
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__Object__) */
