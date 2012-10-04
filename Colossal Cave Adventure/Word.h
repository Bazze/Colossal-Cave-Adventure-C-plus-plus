//
//  Word.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Word__
#define __Colossal_Cave_Adventure__Word__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Word {
  
private:
    int number;
    vector<string> *words;
    string comment;
    
public:
    Word();
    Word(int number, string word, string comment = "");
    virtual ~Word();
    
    //Word(const Word& source);
    
    const int getNumber() const;
    void setNumber(int number);
    
    string getWords();
    void addWord(string word);

    string getComment();
    void setComment(string comment);

    const string showSpec();
    virtual const string toString() = 0;
};

#endif /* defined(__Colossal_Cave_Adventure__Word__) */
