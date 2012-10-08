//
//  LocationCondition.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-08.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__LocationCondition__
#define __Colossal_Cave_Adventure__LocationCondition__

#include <iostream>

class Location;

using namespace std;

class LocationCondition {
    
private:
    int condition;
    Location* from;
    Location* to;
    
public:
    LocationCondition(int cond, Location* from, Location* to);
    virtual ~LocationCondition();
    
    int getCondition() const;
    
    Location* getFromLocation() const;
    Location* getToLocation() const;

};

#endif /* defined(__Colossal_Cave_Adventure__LocationCondition__) */
