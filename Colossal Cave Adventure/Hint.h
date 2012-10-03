//
//  Hint.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-02.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Hint__
#define __Colossal_Cave_Adventure__Hint__

#include <iostream>

#include "Message.h"

class Hint {
  
private:
    int conditionBit;
    int numberOfTurns;
    int pointsToDeduct;
    
    Message* question;
    Message* hint;
    
    /*
     HINT NUMBER (CORRESPONDING TO A COND BIT, SEE SECTION 9)
     - THE NUMBER OF TURNS HE MUST BE AT THE RIGHT LOC(S) BEFORE TRIGGERING THE HINT
     - THE POINTS DEDUCTED FOR TAKING THE HINT
     - THE MESSAGE NUMBER (SECTION 6) OF THE QUESTION
     - AND THE MESSAGE NUMBER OF THE HINT.
     */
    
public:
    Hint(int conditionBit, int numberOfTurns, int pointsToDeduct, Message* question, Message* hint);
    virtual ~Hint();

    const int getConditionBit();
    
    const int getNumberOfTurns();
    
    const int getPointsToDeduct();
    
    const string getQuestion(bool emptyIfNull = true);
    Message* getQuestionObject();

    const string getHint(bool emptyIfNull = true);
    Message* getHintObject();

    const string toString();

};

#endif /* defined(__Colossal_Cave_Adventure__Hint__) */
