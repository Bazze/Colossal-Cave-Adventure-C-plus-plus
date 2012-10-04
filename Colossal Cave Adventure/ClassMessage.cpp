//
//  ClassMessage.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "ClassMessage.h"

#include <sstream>

ClassMessage::ClassMessage(const int number, const string content) : Message(number, content) {
    
}
ClassMessage::ClassMessage(const int number) : Message(number) {
    
}
ClassMessage::~ClassMessage() {
    
}

const string ClassMessage::toString() {
    stringstream s;
    s << "Number:" << endl << this->getNumber() << endl << "Content:" << endl << this->getContent() << endl;
    return s.str();
}