//
//  MotionVerb.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__MotionVerb__
#define __Colossal_Cave_Adventure__MotionVerb__

#include <iostream>
#include <string>

#include "Word.h"

class MotionVerb : public Word {
    
private:
    
public:
    MotionVerb(int number, string word, string comment = "");
    MotionVerb(int number);
    ~MotionVerb();
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__MotionVerb__) */
