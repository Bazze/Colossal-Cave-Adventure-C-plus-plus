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

#include "MotionVerb.h"
#include "Message.h"

Game::Game() {
    // This will parse the data file
    this->data = new Data();
    
    Location* startingLocation = this->data->getLocationByNumber(1);
    this->player = new Player(startingLocation);
    
    this->spokenWords = new vector<Word*>();
}
Game::~Game() {
    delete this->player;
    delete this->spokenWords;
    
    // Important to delete this one last since a vector may not hold pointers to deleted objects
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
    
    // Get the word object for every word written
    Word* word = NULL;
    for (int i = 0; i < lineVector.size(); i++) {
        if ( (word = this->data->findWord(lineVector.at(i))) != NULL) {
            this->spokenWords->push_back(word);
        }
    }
    
    
    if (dynamic_cast<MotionVerb*>(this->spokenWords->at(0))) {
        Message* msg = NULL;
        Location* loc = NULL;
        // Printing messages overrides going to a new location, therefore we check that first
        if ( (msg = this->player->getCurrentLocation()->shouldPrintMessage((MotionVerb*)this->spokenWords->at(0))) != NULL) {
            // Print out message to player
        } else if ( (loc = this->player->getCurrentLocation()->shouldGoToLocation((MotionVerb*)this->spokenWords->at(0))) != NULL ) {
            // Go to new location
        }
    }
    
    // Clear the vector for the next input
    lineVector.clear();
}