//
//  Location.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
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
    this->numberOfVisits = 0;
    this->longDescription = longDescription;
    this->shortDescription = shortDescription;
    this->objects = new vector<Object*>();
    this->assets = vector<bool>();
    for (int i = 0; i < 10; i++) {
        this->assets.push_back(false);
    }
    this->hints = new vector<Hint*>();
    this->motionVerbs = new vector<vector<MotionVerb*>*>();
    this->accessibleLocations = new vector<Location*>();
    this->motionVerbsForPrintingMessage = new vector<vector<MotionVerb*>*>();
    this->printMessages = new vector<Message*>();
    this->locationConditions = new vector<LocationCondition*>();
}

Location::~Location() {
    for (int i = 0; i < this->motionVerbs->size(); i++) {
        delete this->motionVerbs->at(i);
    }
    delete this->motionVerbs;
    delete this->accessibleLocations;
    
    for (int i = 0; i < this->motionVerbsForPrintingMessage->size(); i++) {
        delete this->motionVerbsForPrintingMessage->at(i);
    }
    delete this->motionVerbsForPrintingMessage;
    delete this->printMessages;
    
    delete this->objects;
    delete this->hints;
    delete this->locationConditions;
}

const int Location::getNumber() const {
    return this->number;
}
void Location::setNumber(const int number) {
    this->number = number;
}

int Location::getNumberOfVisits() const {
    return this->numberOfVisits;
}
void Location::visit() {
    this->numberOfVisits++;
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
bool Location::hasObject(Object* obj) const {
    for (int i = 0; i < this->objects->size(); i++) {
        // OK to compare pointers
        if (this->objects->at(i) == obj) {
            return true;
        }
    }
    return false;
}
void Location::removeObject(Object *obj) {
    bool found = false;
    for (int i = 0; i < this->objects->size() && !found; i++) {
        // OK to compare pointers
        if (this->objects->at(i) == obj) {
            this->objects->erase(this->objects->begin()+i);
            found = true;
        }
    }
}
vector<Object*>* Location::getObjects() const {
    return this->objects;
}

bool Location::isAsset(int index) const {
    return this->assets.at(index);
}
void Location::setAsset(const int index, const bool value) {
    if (index >= 0 && index < 10) {
        this->assets.at(index) = value;
    } else {
        // For debugging
        cout << "ERROR: Location class: asset index out of bounds" << endl;
    }
}

void Location::addHint(Hint* h) {
    this->hints->push_back(h);
}
vector<Hint*>* Location::getHints() const {
    return this->hints;
}

// Returns the index of the newly added location
void Location::addAccessibleLocation(Location* loc) {
    // Only add if not already added
    if (this->getAccessibleLocationIndex(loc) == -1) {
        this->accessibleLocations->push_back(loc);
        this->motionVerbs->push_back(new vector<MotionVerb*>());
    }
}
void Location::addMotionVerb(Location* loc, MotionVerb* verb) {
    int index = this->getAccessibleLocationIndex(loc);
    this->motionVerbs->at(index)->push_back(verb);
}
Location* Location::shouldGoToLocation(MotionVerb *verb) {
    for (int i = 0; i < this->motionVerbs->size(); i++) {
        for (int x = 0; x < this->motionVerbs->at(i)->size(); x++) {
            // OK to compare pointers
            if (this->motionVerbs->at(i)->at(x) == verb) {
                return this->accessibleLocations->at(i);
            }
        }
    }
    return NULL;
}

void Location::addPrintMessage(Message* msg) {
    // Only add if not alread added
    if (this->getPrintMessageIndex(msg)) {
        this->printMessages->push_back(msg);
        this->motionVerbsForPrintingMessage->push_back(new vector<MotionVerb*>());
    }
}
void Location::addMotionVerbForPrintMessage(Message* msg, MotionVerb* verb) {
    int index = this->getPrintMessageIndex(msg);
    this->motionVerbsForPrintingMessage->at(index)->push_back(verb);
}
Message* Location::shouldPrintMessage(MotionVerb* verb) {
    for (int i = 0; i < this->motionVerbsForPrintingMessage->size(); i++) {
        for (int x = 0; x < this->motionVerbsForPrintingMessage->at(i)->size(); x++) {
            // OK to compare pointers
            if (this->motionVerbsForPrintingMessage->at(i)->at(x) == verb) {
                return this->printMessages->at(i);
            }
        }
    }
    return NULL;
}

void Location::addLocationCondition(LocationCondition *locC) {
    this->locationConditions->push_back(locC);
}
LocationCondition* Location::getLocationConditionForLocation(Location *loc) const {
    for (int i = 0; i < this->locationConditions->size(); i++) {
        // OK to compare pointers
        if (this->locationConditions->at(i)->getToLocation() == loc) {
            return this->locationConditions->at(i);
        }
    }
    return NULL;
}
LocationCondition* Location::getLocationConditionAfterLocationCondition(LocationCondition* locC) const {
    for (int i = 0; i < this->locationConditions->size(); i++) {
        // OK to compare pointers
        if (this->locationConditions->at(i) == locC) {
            if (i != this->locationConditions->size()-1) {
                return this->locationConditions->at(i+1);
            }
        }
    }
    return NULL;
}

string Location::listObjects() const {
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
const int Location::getPrintMessageIndex(Message *msg) const {
    for (int i = 0; i < this->printMessages->size(); i++) {
        if (this->printMessages->at(i)->getNumber() == msg->getNumber()) {
            return i;
        }
    }
    return -1;
}

string Location::getAccessibleLocationsAndMotionVerbsAsString() const {
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
string Location::getPrintMessagesAndMotionVerbsAsString() const {
    stringstream ss;
    ss << "Print messages:" << endl;
    if (this->printMessages->size() > 0) {
        for (int i = 0; i < this->printMessages->size(); i++) {
            ss << this->printMessages->at(i)->getNumber() << " [" << this->printMessages->at(i)->getContent() << "]" << endl;
            ss << "\tMotionVerbs:" << endl;
            if (this->motionVerbsForPrintingMessage->at(i)->size() > 0) {
                for (int x = 0; x < this->motionVerbsForPrintingMessage->at(i)->size(); x++) {
                    ss << "\t\t" << this->motionVerbsForPrintingMessage->at(i)->at(x)->getNumber() << ": " << this->motionVerbsForPrintingMessage->at(i)->at(x)->getWords() << endl;
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
string Location::getAssetsAsString() const {
    stringstream ss;
    ss << "Assets:" << endl;
    for (int i = 0; i < this->assets.size(); i++) {
        ss << "\t" << i << ": " << (this->assets.at(i) ? "true" : "false") << endl;
    }
    return ss.str();
}
string Location::getHintsAsString() {
    stringstream ss;
    ss << "Hints:" << endl;
    if (this->hints->size() > 0) {
        for (int i = 0; i < this->hints->size(); i++) {
            if (this->hints->at(i)->getHintObject() != NULL) {
                ss << "\t" << this->hints->at(i)->getHintObject()->getNumber() << ": " << this->hints->at(i)->getHintObject()->getContent() << endl;
            }
        }
    } else {
        ss << "\t<none>" << endl;
    }
    return ss.str();
}

string Location::toString() {
    stringstream s;
    s << "Id: " << this->getNumber() << endl << "Short description:" << endl << (this->getShortDescription() == "" ? "<empty>" : this->getShortDescription()) << endl << "Long description:" << endl << this->getLongDescription() << endl << "Objects:" << endl << this->listObjects() << endl << this->getHintsAsString() << this->getAccessibleLocationsAndMotionVerbsAsString() << this->getPrintMessagesAndMotionVerbsAsString() << this->getAssetsAsString();
    return s.str();
}