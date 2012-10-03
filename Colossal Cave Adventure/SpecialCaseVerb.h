//
//  SpecialCaseVerb.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-09-29.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__SpecialCaseVerb__
#define __Colossal_Cave_Adventure__SpecialCaseVerb__

#include <iostream>
#include <string>

#include "Word.h"

class SpecialCaseVerb : public Word {
    
private:
    
public:
    SpecialCaseVerb(int number, string word, string comment = "");
    ~SpecialCaseVerb();
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__SpecialCaseVerb__) */
