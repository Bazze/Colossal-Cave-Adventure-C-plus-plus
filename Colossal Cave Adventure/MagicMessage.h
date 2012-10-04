//
//  MagicMessage.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__MagicMessage__
#define __Colossal_Cave_Adventure__MagicMessage__

#include <iostream>

#include "Message.h"

class MagicMessage : public Message {
    
private:
    
    
public:
    MagicMessage(const int number, const string content);
    MagicMessage(const int number);
    ~MagicMessage();
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__MagicMessage__) */
