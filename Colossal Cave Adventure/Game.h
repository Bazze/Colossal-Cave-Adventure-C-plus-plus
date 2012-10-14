//
//  Game.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-10-05.
//  Copyright (c) 2012. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Game__
#define __Colossal_Cave_Adventure__Game__

#include <iostream>
#include <string>
#include <vector>

#include "Player.h"
#include "Location.h"
#include "Data.h"

class Game {
  
private:
    Player* player;
    
    Data* data;
    
    vector<string> &split(const string &s, char delim, vector<string> &elems, int lengthPerWord = 5);
    
    string getHint(Location* loc) const;
    
    string handleMovementAndLook(vector<Word*> &spokenWords, MotionVerb* verb);
    
public:
    Game();
    virtual ~Game();
    
    Player* getPlayer() const;
    
    string parseInput(string input);

};

#endif /* defined(__Colossal_Cave_Adventure__Game__) */
