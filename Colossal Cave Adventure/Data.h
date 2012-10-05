//
//  Data.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Data__
#define __Colossal_Cave_Adventure__Data__

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>

#include "Location.h"
#include "Word.h"
#include "Object.h"
#include "Message.h"
#include "ClassMessage.h"
#include "MagicMessage.h"
#include "ActionVerb.h"
#include "MotionVerb.h"
#include "Hint.h"


using namespace std;

class Data {
  
private:
    vector<Location*> *locations;
    vector<Word*> *words;
    vector<Message*> *messages;
    vector<MagicMessage*> *magicMessages;
    vector<ClassMessage*> *classMessages;
    vector<Hint*> *hints;
    
    vector<string> &split(const string &s, char delim, vector<string> &elems);
    
    /*const string trim(const string& pString, const string& pWhitespace = " ");
    const string reduce(const string& pString, const string& pFill = "", const string& pWhitespace = " ");*/
    
    void parseLines(ifstream &dataFile);
    
    void deallocLocations();
    void deallocWords();
    void deallocMessages();
    void deallocMagicMessages();
    void deallocClassMessages();
    void deallocHints();
    
    void loadData(const string filename);
    
public:
    Data(string filename = "advent.dat");
    virtual ~Data();
    
    // Could return any of the subclasses of Word as well
    Word* findWord(string word) const;
    
    Object* getObjectByNumber(const int n);
    Location* getLocationByNumber(const int n);
    ActionVerb* getActionVerbByNumber(const int n);
    MotionVerb* getMotionVerbByNumber(const int n);
    Message* getMessageByNumber(const int n);
    
    // For debugging purposes
    void dumpAllLocations();
    void dumpAllWords();
    void dumpAllMessages();
    void dumpAllMagicMessages();
    void dumpAllClassMessages();
    void dumpAllHints();
    
};

#endif /* defined(__Colossal_Cave_Adventure__Data__) */
