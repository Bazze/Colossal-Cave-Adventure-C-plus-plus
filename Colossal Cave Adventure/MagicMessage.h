//
//  MagicMessage.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-03.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__MagicMessage__
#define __Colossal_Cave_Adventure__MagicMessage__

#include <iostream>

#include "Message.h"

class MagicMessage : public Message {
    
private:
    
    
public:
    MagicMessage(const int number, const string content);
    ~MagicMessage();
    
};

#endif /* defined(__Colossal_Cave_Adventure__MagicMessage__) */
