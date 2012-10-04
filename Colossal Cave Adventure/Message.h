//
//  Message.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Message__
#define __Colossal_Cave_Adventure__Message__

#include <iostream>
#include <string>

using namespace std;

class Message {
    
private:
    int number;
    string content;
    
public:
    Message(const int number, const string content);
    Message(const int number);
    virtual ~Message();
    
    const int getNumber();
    void setNumber(const int number);
    
    const string getContent();
    void setContent(const string content);
    void appendContent(const string content);
    
    virtual const string toString();
};

#endif /* defined(__Colossal_Cave_Adventure__Message__) */
