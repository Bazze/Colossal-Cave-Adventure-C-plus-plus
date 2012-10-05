//
//  Game.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling on 2012-10-05.
//  Copyright (c) 2012 SN Solutions. All rights reserved.
//

#include <string>
#include <sstream>
#include <vector>
#include "Game.h"

Game::Game() {
    this->player = new Player();
    
    // This will parse the data file
    this->data = new Data();
}
Game::~Game() {
    delete this->player;
    delete this->data;
}

/* Split function from: http://stackoverflow.com/questions/236129/splitting-a-string-in-c#answer-236803 */
vector<string>& Game::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
/* End split */

void Game::parseInput(string input) {
    vector<string> lineVector;
    
    this->split(input, ' ', lineVector);
    
    for (int i = 0; i < lineVector.size(); i++) {
        this->data->findWord(lineVector.at(i));
    }
}