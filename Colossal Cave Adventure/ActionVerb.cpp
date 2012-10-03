//
//  ActionVerb.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-09-29.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#include "ActionVerb.h"

ActionVerb::ActionVerb(int number, string word, string comment) : Word(number, word, comment) {
    this->defaultMessage = NULL;
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

const string ActionVerb::toString() {
    return "Type: ActionVerb\n" + this->showSpec();
}