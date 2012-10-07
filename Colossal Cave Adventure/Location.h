//
//  Location.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Location__
#define __Colossal_Cave_Adventure__Location__

#include <iostream>
#include <string>
#include <vector>
#include "MotionVerb.h"
#include "Object.h"
#include "Message.h"
#include "Hint.h"

using namespace std;

class Location {
    
private:
    int number;
    int numberOfVisits;
    string shortDescription;
    string longDescription;
    
    vector<Object*> *objects;
    vector<bool> assets;
    
    vector<Hint*> *hints;
    
    vector<Location*> *accessibleLocations;
    vector<vector<MotionVerb*>*> *motionVerbs;
    
    vector<Message*> *printMessages;
    vector<vector<MotionVerb*>*> *motionVerbsForPrintingMessage;
    
    void init(int number = -1, string longDescription = "", string shortDescription = "");
    
    const int getAccessibleLocationIndex(Location* loc) const;
    const int getPrintMessageIndex(Message* msg) const;
    
    string listObjects() const;
    string getAccessibleLocationsAndMotionVerbsAsString() const;
    string getPrintMessagesAndMotionVerbsAsString() const;
    string getAssetsAsString() const;
    string getHintsAsString();
    
public:
    Location();
    Location(int number);
    Location(int number, string longDescription);
    Location(int number, string longDescription, string shortDescription);
    ~Location();
    
    const int getNumber() const;
    void setNumber(const int number);
    
    int getNumberOfVisits() const;
    void visit();
    
    const string getShortDescription();
    void setShortDescription(const string desc);
    void appendToShortDescription(const string desc);
    
    const string getLongDescription();
    void setLongDescription(const string desc);
    void appendToLongDescription(const string desc);
    
    void addObject(Object* obj);
    bool hasObject(Object* obj) const;
    void removeObject(Object* obj);
    
    bool isAsset(int index) const;
    void setAsset(const int index, const bool value = true);
    
    void addHint(Hint* h);
    vector<Hint*>* getHints() const;
    
    void addAccessibleLocation(Location* loc);
    void addMotionVerb(Location* loc, MotionVerb* verb);
    Location* shouldGoToLocation(MotionVerb* verb);
    
    void addPrintMessage(Message* msg);
    void addMotionVerbForPrintMessage(Message* msg, MotionVerb* verb);
    Message* shouldPrintMessage(MotionVerb* verb);
    
    string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__Location__) */
