//
//  MotionVerb.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "MotionVerb.h"

MotionVerb::MotionVerb(int number, string word, string comment) : Word(number, word, comment) {
    
}

MotionVerb::MotionVerb(int number) : Word(number, "", "") {
    
}

MotionVerb::~MotionVerb() {
    
}

const string MotionVerb::toString() {
    return "Type: MotionVerb\n" + this->showSpec();
}