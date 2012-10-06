//
//  ActionVerb.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "ActionVerb.h"

ActionVerb::ActionVerb(int number, string word, string comment) : Word(number, word, comment) {
    this->defaultMessage = NULL;
    this->dropsObject = false;
    this->picksUpObject = false;
}

ActionVerb::~ActionVerb() {
    // The data class will take care of this
    // delete this->defaultMessage;
}

void ActionVerb::setDefaultMessage(Message* msg) {
    
}
const string ActionVerb::getDefaultMessage() {
    if (this->defaultMessage != NULL) {
        return this->defaultMessage->getContent();
    } else {
        return "[No message object attached to this ActionVerb]";
    }
}
Message* ActionVerb::getDefaultMessageObject() {
    return this->defaultMessage;
}

bool ActionVerb::doesDropObject() const {
    return this->dropsObject;
}
bool ActionVerb::doesPickUpObject() const {
    return this->picksUpObject;
}
void ActionVerb::setDropsObject(bool drop) {
    this->dropsObject = drop;
}
void ActionVerb::setPicksUpObject(bool pickUp) {
    this->picksUpObject = pickUp;
}

const string ActionVerb::toString() {
    string s = "Type: ActionVerb\nPicks up object:\n";
    s += (this->doesPickUpObject() == true ? "Yes" : "No");
    s += "\nDrops object:\n";
    s += (this->doesPickUpObject() == true ? "Yes" : "No");
    s += "\n" + this->showSpec();
    return s;
}