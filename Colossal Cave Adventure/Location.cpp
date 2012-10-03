//
//  Location.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-09-29.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#include "Location.h"
#include "Object.h"

#include <sstream>
#include <vector>
#include <iostream>

Location::Location() {
    this->init();
}
Location::Location(int number) {
    this->init(number);
}
Location::Location(int number, string longDescription) {
    this->init(number, longDescription);
}
Location::Location(int number, string longDescription, string shortDescription) {
    this->init(number, longDescription, shortDescription);
}

void Location::init(int number, string longDescription, string shortDescription) {
    this->number = number;
    this->longDescription = longDescription;
    this->shortDescription = shortDescription;
    this->objects = new vector<Object*>();
    this->assets = vector<bool>();
    for (int i = 0; i < 10; i++) {
        this->assets.push_back(false);
    }
    this->motionVerbs = new vector<vector<MotionVerb*>*>();
    this->accessibleLocations = new vector<Location*>();
}

Location::~Location() {
    for (int i = 0; i < this->motionVerbs->size(); i++) {
        delete this->motionVerbs->at(i);
    }
    delete this->motionVerbs;
    delete this->objects;
    delete this->accessibleLocations;
}

const int Location::getNumber() const {
    return this->number;
}
void Location::setNumber(const int number) {
    this->number = number;
}

const string Location::getShortDescription() {
    return this->shortDescription;
}
void Location::setShortDescription(const string desc) {
    this->shortDescription = desc;
}
void Location::appendToShortDescription(const string desc) {
    this->shortDescription += desc + " ";
}

const string Location::getLongDescription() {
    return this->longDescription;
}
void Location::setLongDescription(const string desc) {
    this->longDescription = desc;
}
void Location::appendToLongDescription(const string desc) {
    this->longDescription += desc + " ";
}

void Location::addObject(Object* obj) {
    this->objects->push_back(obj);
}

void Location::setAsset(const int index, const bool value) {
    if (index >= 0 && index < 10) {
        this->assets.at(index) = value;
    } else {
        // For debugging
        cout << "ERROR: Location class: asset index out of bounds" << endl;
    }
}

// Returns the index of the newly added location
void Location::addAccessibleLocation(Location* loc) {
    this->accessibleLocations->push_back(loc);
    this->motionVerbs->push_back(new vector<MotionVerb*>());
}
void Location::addMotionVerb(Location* loc, MotionVerb* verb) {
    int index = this->getAccessibleLocationIndex(loc);
    this->motionVerbs->at(index)->push_back(verb);
    /*if (index != -1) {
        // If no words have been added for this location, initialize new vector first
        if (this->motionVerbs->size() <= index) {
            this->motionVerbs->push_back(new vector<MotionVerb*>());
        }
        this->motionVerbs->at(index)->push_back(verb);
    } else {
        cout << "ERROR: Location class: addMotionVerb(): location not found." << endl;
    }*/
}

const string Location::listObjects() {
    stringstream s;
    if (this->objects->size() > 0) {
        for (int i = 0; i < this->objects->size(); i++) {
            s << this->objects->at(i)->getNumber() << ":[" << this->objects->at(i)->getWords() << "]";
            if (i != this->objects->size()-1) {
                s << " ";
            }
        }
    } else {
        s << "<none>";
    }
    return s.str();
}

const int Location::getAccessibleLocationIndex(Location* loc) const {
    for (int i = 0; i < this->accessibleLocations->size(); i++) {
        if (this->accessibleLocations->at(i)->getNumber() == loc->getNumber()) {
            return i;
        }
    }
    return -1;
}

const string Location::getAccessibleLocationsAndMotionVerbsAsString() {
    stringstream ss;
    ss << "Accessible locations:" << endl;
    if (this->accessibleLocations->size() > 0) {
        for (int i = 0; i < this->accessibleLocations->size(); i++) {
            ss << this->accessibleLocations->at(i)->getNumber() << " [" << this->accessibleLocations->at(i)->getShortDescription() << "]" << endl;
            ss << "\tMotionVerbs:" << endl;
            if (this->motionVerbs->at(i)->size() > 0) {
                for (int x = 0; x < this->motionVerbs->at(i)->size(); x++) {
                    ss << "\t\t" << this->motionVerbs->at(i)->at(x)->getNumber() << ": " << this->motionVerbs->at(i)->at(x)->getWords() << endl;
                }
            } else {
                ss << "\t" << "<none>" << endl;
            }
        }
    } else {
        ss << "<none>" << endl;
    }
    return ss.str();
}

const string Location::toString() {
    stringstream s;
    s << "Id: " << this->getNumber() << endl << "Short description:" << endl << (this->getShortDescription() == "" ? "<empty>" : this->getShortDescription()) << endl << "Long description:" << endl << this->getLongDescription() << endl << "Objects:" << endl << this->listObjects() << endl << this->getAccessibleLocationsAndMotionVerbsAsString();
    return s.str();
}