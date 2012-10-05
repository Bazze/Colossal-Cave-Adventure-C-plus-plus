//
//  Game.h
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-05.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#ifndef __Colossal_Cave_Adventure__Game__
#define __Colossal_Cave_Adventure__Game__

#include <iostream>
#include <string>

#include "Player.h"
#include "Data.h"

class Game {
  
private:
    Player* player;
    
    Data* data;
    
    vector<string> &split(const string &s, char delim, vector<string> &elems);
    
public:
    Game();
    virtual ~Game();
    
    void parseInput(string input);

};

#endif /* defined(__Colossal_Cave_Adventure__Game__) */
