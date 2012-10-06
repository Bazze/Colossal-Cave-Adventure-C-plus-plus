//
//  Object.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
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
    
    // For Light object
    bool lightable;
    bool lit;
    
public:
    Object(int number, string word, string comment = "");
    ~Object();
    
    const int getPropertyValue();
    void setPropertyValue(const int propertyValue);
    
    const string getInventoryMessage() const;
    void setInventoryMessage(const string msg);
    
    void setDescriptionsVector(vector<string> *vector);
    const string getDescriptionForPropertyValue(const int n) const;
    const string getPropertyDescriptions() const;
    void addPropertyDescription(const string desc);
    void appendToPropertyDescription(const int n, const string desc);
    
    bool isTreasure() const;
    void setTreasure(const bool treasure);
    
    bool isMovable() const;
    void setMovable(const bool movable);
    
    bool isLightable() const;
    void setLightable(bool l);
    
    bool isLit() const;
    void setLit(bool l);
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__Object__) */
