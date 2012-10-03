//
//  MotionVerb.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-09-29.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
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
    ~MotionVerb();
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__MotionVerb__) */
