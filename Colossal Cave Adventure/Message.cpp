//
//  Message.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "Message.h"

#include <sstream>

Message::Message(const int number, const string content) {
    this->number = number;
    this->content = content;
}
Message::Message(const int number) {
    this->number = number;
    this->content = "";
}
Message::~Message() {
    
}

int Message::getNumber() const {
    return this->number;
}
void Message::setNumber(const int number) {
    this->number = number;
}

const string Message::getContent() {
    return this->content;
}
void Message::setContent(const string content) {
    this->content = content;
}
void Message::appendContent(const string content) {
    this->content += content + " ";
}

const string Message::toString() {
    stringstream s;
    s << "Number:" << endl << this->getNumber() << endl << "Content:" << endl << this->getContent() << endl;
    return s.str();
}