//
//  Hint.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "Hint.h"

#include <sstream>

Hint::Hint(int conditionBit, int numberOfTurns, int pointsToDeduct, Message* question, Message* hint) {
    this->conditionBit = conditionBit;
    this->numberOfTurns = numberOfTurns;
    this->pointsToDeduct = pointsToDeduct;
    this->question = question;
    this->hint = hint;
}

Hint::~Hint() {
    
}

const int Hint::getConditionBit() {
    return this->conditionBit;
}

const int Hint::getNumberOfTurns() {
    return this->numberOfTurns;
}

const int Hint::getPointsToDeduct() {
    return this->pointsToDeduct;
}

const string Hint::getQuestion(bool emptyIfNull) {
    if (this->question != NULL) {
        return this->question->getContent();
    } else {
        if (emptyIfNull) {
            return "";
        } else {
            return "[No message object attached to this question.]";
        }
    }
}
Message* Hint::getQuestionObject() {
    return this->question;
}

const string Hint::getHint(bool emptyIfNull) {
    if (this->hint != NULL) {
        return this->hint->getContent();
    } else {
        if (emptyIfNull) {
            return "";
        } else {
            return "[No message object attached to this hint.]";
        }
    }
}
Message* Hint::getHintObject() {
    return this->hint;
}

const string Hint::toString() {
    stringstream ss;
    ss << "Condition bit:" << endl << this->getConditionBit() << endl << "Number of turns:" << endl << this->getNumberOfTurns() << endl;
    ss << "Points to deduct: " << endl << this->getPointsToDeduct() << endl << "Question:" << endl << this->getQuestion(false) << endl;
    ss << "Hint:" << endl << this->getHint(false) << endl;
    return ss.str();
}