//
//  ClassMessage.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-02.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__ClassMessage__
#define __Colossal_Cave_Adventure__ClassMessage__

#include <iostream>
#include "Message.h"

class ClassMessage : public Message {
    
private:
    
    
public:
    ClassMessage(const int number, const string content);
    ClassMessage(const int number);
    ~ClassMessage();
    
    const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__ClassMessage__) */
