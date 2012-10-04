//
//  ActionVerb.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__ActionVerb__
#define __Colossal_Cave_Adventure__ActionVerb__

#include <iostream>
#include <string>

#include "Word.h"
#include "Message.h"

class ActionVerb : public Word {
    
private:
    Message* defaultMessage;
    
public:
    ActionVerb(int number, string word, string comment = "");
    ~ActionVerb();
    
    void setDefaultMessage(Message* msg);
    const string getDefaultMessage();
    Message* getDefaultMessageObject();
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__ActionVerb__) */
