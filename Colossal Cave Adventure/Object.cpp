//
//  Object.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "Object.h"

#include <sstream>

Object::Object(int number, string word, string comment) : Word(number, word, comment) {
    this->propertyValue = 0;
    this->inventoryMessage = "";
    this->descriptions = new vector<string>();
    this->movable = true;
}

Object::~Object() {
    delete this->descriptions;
}

const int Object::getPropertyValue() {
    return this->propertyValue;
}
void Object::setPropertyValue(const int propertyValue) {
    this->propertyValue = propertyValue;
}

const string Object::getInventoryMessage() {
    return this->inventoryMessage;
}
void Object::setInventoryMessage(const string msg) {
    this->inventoryMessage = msg;
}

void Object::setDescriptionsVector(vector<string> *vector) {
    delete this->descriptions;
    this->descriptions = vector;
}
const string Object::getDescriptionForPropertyValue(const int n) {
    if (this->descriptions->size() >= n) {
        return this->descriptions->at(n);
    }
    return "";
}
const string Object::getPropertyDescriptions() {
    stringstream s;
    for (int i = 0; i < this->descriptions->size(); i++) {
        s << endl << "\t" << i << ": " << this->descriptions->at(i);
    }
    return s.str();
}
void Object::addPropertyDescription(const string desc) {
    this->descriptions->push_back(desc);
}
void Object::appendToPropertyDescription(const int n, const string desc) {
    if (this->descriptions->size() >= n+1) {
        this->descriptions->at(n) += desc;
    }
}

const bool Object::isTreasure() {
    return this->treasure;
}
void Object::setTreasure(const bool treasure) {
    this->treasure = treasure;
}

const bool Object::isMovable() {
    return this->movable;
}
void Object::setMovable(const bool movable) {
    this->movable = movable;
}

const string Object::toString() {
    return "Type: Object\n" + this->showSpec() + "Treasure:\n" + (this->isTreasure() ? "Yes" : "No") + "\nMovable:\n" + (this->isMovable() ? "Yes" : "No") + "\nInventory message:\n" + this->getInventoryMessage() + "\nProperty descriptions:" + this->getPropertyDescriptions();
}