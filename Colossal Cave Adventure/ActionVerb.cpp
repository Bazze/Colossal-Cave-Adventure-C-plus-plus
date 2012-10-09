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
}

ActionVerb::~ActionVerb() {
    
}

void ActionVerb::setDefaultMessage(Message* msg) {
    this->defaultMessage = msg;
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

const string ActionVerb::toString() {
    return "Type: ActionVerb\nDefault message:\n" + this->getDefaultMessage() + "\n" + this->showSpec();
}