//
//  MagicMessage.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-03.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#include "MagicMessage.h"

#include <sstream>

MagicMessage::MagicMessage(const int number, const string content) : Message(number, content) {
    
}
MagicMessage::MagicMessage(const int number) : Message(number) {
    
}
MagicMessage::~MagicMessage() {
    
}

const string MagicMessage::toString() {
    stringstream s;
    s << "Number:" << endl << this->getNumber() << endl << "Content:" << endl << this->getContent() << endl;
    return s.str();
}