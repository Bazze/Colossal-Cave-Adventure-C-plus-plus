//
//  LocationCondition.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-08.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#include "LocationCondition.h"

LocationCondition::LocationCondition(int cond, Location* from, Location* to) {
    this->condition = cond;
    this->from = from;
    this->to = to;
}
LocationCondition::~LocationCondition() {
    
}

int LocationCondition::getCondition() const {
    return this->condition;
}

Location* LocationCondition::getFromLocation() const {
    return this->from;
}
Location* LocationCondition::getToLocation() const {
    return this->to;
}