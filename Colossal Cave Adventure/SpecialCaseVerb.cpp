//
//  SpecialCaseVerb.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "SpecialCaseVerb.h"

SpecialCaseVerb::SpecialCaseVerb(int number, string word, string comment) : Word(number, word, comment) {
    
}

SpecialCaseVerb::~SpecialCaseVerb() {
    
}

const string SpecialCaseVerb::toString() {
    return "Type: SpecialCaseVerb\n" + this->showSpec();
}