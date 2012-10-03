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

Location::~Location() {
    // The Data class will take care of the deallocation of Objects
    //this->deallocObjects();
    delete this->objects;
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
}

/*
void Location::deallocObjects() {
    for (int i = 0; i < this->objects->size(); i++) {
        delete this->objects->at(i);
    }
    delete this->objects;
}
*/

const int Location::getNumber() {
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
string Location::toString() {
    stringstream s;
    s << "Id: " << this->getNumber() << endl << "Short description:" << endl << (this->getShortDescription() == "" ? "<empty>" : this->getShortDescription()) << endl << "Long description:" << endl << this->getLongDescription() << endl << "Objects:" << endl << this->listObjects() << endl;
    return s.str();
}