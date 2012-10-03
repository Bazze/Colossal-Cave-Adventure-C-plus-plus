//
//  ClassMessage.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-02.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
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