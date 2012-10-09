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
#include "ActionVerb.h"
#include "Message.h"
#include "Object.h"
#include "Hint.h"
#include "Location.h"
#include "LocationCondition.h"

Game::Game() {
    // This will parse the data file
    this->data = new Data();
    
    Location* startingLocation = this->data->getLocationByNumber(1);
    this->player = new Player(startingLocation);
}
Game::~Game() {
    delete this->player;
    
    // Important to delete this one last since a vector may not hold pointers to deleted objects
    delete this->data;
}

// Split function from: http://stackoverflow.com/questions/236129/splitting-a-string-in-c#answer-236803
// Added: length per word
vector<string>& Game::split(const string &s, char delim, vector<string> &elems, int lengthPerWord) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item.substr(0, lengthPerWord));
    }
    return elems;
}
/* End split */

Player* Game::getPlayer() const {
    return this->player;
}

string Game::getHint(Location* loc) const {
    vector<Hint*>* hints = loc->getHints();
    for (int i = 0; i < hints->size(); i++) {
        if (!hints->at(i)->hasBeenRead() && loc->getNumberOfVisits() >= hints->at(i)->getNumberOfTurns()) {
            hints->at(i)->setRead(true);
            return "\n\n-- HINT --\n " + hints->at(i)->getQuestion() + "\n" + hints->at(i)->getHint();
        }
    }
    return "";
}

string Game::parseInput(string input) {
    vector<string> lineVector;
    vector<Word*> spokenWords = vector<Word*>();
    
    this->split(input, ' ', lineVector, 5);
    
    // Get the word object for every word written
    Word* word = NULL;
    for (int i = 0; i < lineVector.size(); i++) {
        if ( (word = this->data->findWord(lineVector.at(i))) != NULL) {
            spokenWords.push_back(word);
        } else {
            // For debugging
            cout << "Word not found: " << lineVector.at(i) << endl;
        }
    }
    
    if (spokenWords.size() > 0) {
        if (spokenWords.size() == 1) {
            if (dynamic_cast<MotionVerb*>(spokenWords.at(0))) {
                MotionVerb* verb = (MotionVerb*)spokenWords.at(0);
                Message* msg = NULL;
                Location* loc = NULL;
                switch (verb->getNumber()) {
                    // 57   LOOK EXAMI TOUCH DESCR
                    case 57:
                    {
                        loc = this->player->getCurrentLocation();
                        vector<Object*> *objects = loc->getObjects();
                        string message = "";
                        for (int i = 0; i < objects->size(); i++) {
                            message += (i != 0 ? "\n\n" : "") + objects->at(i)->getCurrentPropertyDescription();
                        }
                        
                        if (message == "") {
                            message = "You see nothing out of the ordinary.";
                        }
                        
                        return message;
                    }
                    break;
                        
                    default:
                    {
                        // Printing messages overrides going to a new location, therefore we check that first
                        if ( (msg = this->player->getCurrentLocation()->shouldPrintMessage(verb)) != NULL) {
                            // Print out message to player
                            return msg->getContent();
                        } else if ( (loc = this->player->getCurrentLocation()->shouldGoToLocation(verb)) != NULL ) {
                            bool allowed = false;
                            string msg = "";
                            
                            // Let see if all conditions are met
                            LocationCondition* cond = this->player->getCurrentLocation()->getLocationConditionForLocation(loc);
                            if (cond != NULL) {
                                int M = cond->getCondition();
                                
                                // IF M=0		IT'S UNCONDITIONAL.
                                if (M == 0) {
                                    allowed = true;
                                }
                                // IF 0<M<100	IT IS DONE WITH M% PROBABILITY.
                                else if (M > 0 && M < 100) {
                                    srand((unsigned)time(0));
                                    int randInt = (rand()%100)+1;
                                    if (randInt <= M) {
                                        allowed = true;
                                    }
                                }
                                // IF M=100	UNCONDITIONAL, BUT FORBIDDEN TO DWARVES.
                                else if (M == 100) {
                                    // Dwarves?
                                    allowed = true;
                                }
                                // IF 100<M<=200	HE MUST BE CARRYING OBJECT M-100.
                                else if (M > 100 && M <= 200) {
                                    Object* obj = this->data->getObjectByNumber(M-100);
                                    if (obj != NULL) {
                                        if (this->player->hasObject(obj)) {
                                            allowed = true;
                                        } else {
                                            msg = "You must be carrying " + obj->getInventoryMessage();
                                        }
                                    }
                                }
                                // IF 200<M<=300	MUST BE CARRYING OR IN SAME ROOM AS M-200.
                                else if (M > 200 && M <= 300) {
                                    Object* obj = this->data->getObjectByNumber(M-200);
                                    if (obj != NULL) {
                                        if (this->player->hasObject(obj) || this->player->getCurrentLocation()->hasObject(obj)) {
                                            allowed = true;
                                        } else {
                                            msg = "You must be carrying or be in the same room as " + obj->getInventoryMessage();
                                        }
                                    }
                                }
                                // IF 300<M<=400	PROP(M MOD 100) MUST *NOT* BE 0.
                                else if (M > 300 && M <= 400) {
                                    Object* obj = this->data->getObjectByNumber(M%100);
                                    if (obj != NULL) {
                                        if (obj->getPropertyValue() != 0) {
                                            allowed = true;
                                        } else {
                                            msg = "Property value must not be 0.";
                                        }
                                    }
                                }
                                // IF 400<M<=500	PROP(M MOD 100) MUST *NOT* BE 1.
                                else if (M > 400 && M <= 500) {
                                    Object* obj = this->data->getObjectByNumber(M%100);
                                    if (obj != NULL) {
                                        if (obj->getPropertyValue() != 1) {
                                            allowed = true;
                                        } else {
                                            msg = "Property value must not be 1.";
                                        }
                                    }
                                }
                                // IF 500<M<=600	PROP(M MOD 100) MUST *NOT* BE 2, ETC.
                                else if (M > 500 && M <= 600) {
                                    Object* obj = this->data->getObjectByNumber(M%100);
                                    if (obj != NULL) {
                                        if (obj->getPropertyValue() != 2) {
                                            allowed = true;
                                        } else {
                                            msg = "Property value must not be 2.";
                                        }
                                    }
                                }
                            } else {
                                allowed = true;
                            }
                            
                            if (allowed) {
                                // Go to new location
                                this->player->setCurrentLocation(loc);
                                return loc->getShortDescription() + (loc->getShortDescription() != "" ? "\n" : "") + loc->getLongDescription() + this->getHint(loc);
                            } else {
                                return msg;
                            }
                        }
                    }
                    break;
                }
            } else if (dynamic_cast<ActionVerb*>(spokenWords.at(0))) {
                ActionVerb* verb = (ActionVerb*)spokenWords.at(0);
                return verb->getDefaultMessage();
            }
        }
        else if (spokenWords.size() == 2) {
            if (dynamic_cast<ActionVerb*>(spokenWords.at(0))) {
                ActionVerb* verb = (ActionVerb*)spokenWords.at(0);
                
                // "goto building", "<ActionVerb> <MotionVerb>"
                if (dynamic_cast<MotionVerb*>(spokenWords.at(1))) {
                    
                }
                // "get keys", "off lamp", "<ActionVerb> <Object>"
                else if (dynamic_cast<Object*>(spokenWords.at(1))) {
                    Object* obj = (Object*)spokenWords.at(1);
                    
                    /*
                     -2001 CARRY TAKE KEEP CATCH STEAL CAPTU GET TOTE
                     -2002 DROP RELEA FREE DISCA DUMP
                     2003 SAY CHANT SING UTTER MUMBL
                     -2004 UNLOC OPEN
                     2005 NOTHI
                     -2006 LOCK CLOSE
                     -2007 LIGHT ON
                     -2008 EXTIN OFF
                     -2009 WAVE SHAKE SWING
                     -2010 CALM PLACA TAME
                     2011 WALK RUN TRAVE GO PROCE CONTI EXPLO GOTO FOLLO TURN
                     -2012 ATTAC KILL FIGHT HIT STRIK
                     2013 POUR
                     -2014 EAT DEVOU
                     -2015 DRINK
                     -2016 RUB
                     -2017 THROW TOSS
                     2018 QUIT
                     2019 FIND WHERE
                     2020 INVEN
                     2021 FEED
                     -2022 FILL
                     2023 BLAST DETON IGNIT BLOWU
                     2024 SCORE
                     2025 FEE FIE FOE FOO FUM
                     2026 BRIEF
                     2027 READ PERUS
                     2028 BREAK SHATT SMASH
                     2029 WAKE DISTU
                     2030 SUSPE PAUSE SAVE
                     2031 HOURS
                     */
                    switch (verb->getNumber()) {
                        // 2001 CARRY TAKE KEEP CATCH STEAL CAPTU GET TOTE
                        case 2001:
                            if (this->player->getCurrentLocation()->hasObject(obj)) {
                                // Pick up the object from the current location
                                this->player->getCurrentLocation()->removeObject(obj);
                                // Add it to the player
                                this->player->pickUpObject(obj);
                                return "You picked up: " + obj->getInventoryMessage();
                            } else {
                                return "There is no such object at this location.";
                            }
                        break;
                            
                        // 2002 DROP RELEA FREE DISCA DUMP
                        case 2002:
                            if (this->player->hasObject(obj)) {
                                // Drop object
                                this->player->dropObject(obj);
                                // Leave the object at the current location
                                this->player->getCurrentLocation()->addObject(obj);
                                return "You dropped: " + obj->getInventoryMessage();
                            } else {
                                return "You don't have this object in your inventory.";
                            }
                        break;
                            
                        // 2004 UNLOC OPEN
                        case 2004:
                            if (obj->getNumber() == 1014 /* CLAM */) {
                                // 1018	KNIFE KNIVE
                                if (this->player->hasObject(this->data->getObjectByNumber(18))) {
                                    if (this->player->getCurrentLocation()->hasObject(obj)) {
                                        // 124	A GLISTENING PEARL FALLS OUT OF THE CLAM AND ROLLS AWAY.  GOODNESS,
                                        // 124	THIS MUST REALLY BE AN OYSTER.	(I NEVER WAS VERY GOOD AT IDENTIFYING
                                        // 124	BIVALVES.)  WHATEVER IT IS, IT HAS NOW SNAPPED SHUT AGAIN.
                                        return this->data->getMessageByNumber(124)->getContent();
                                    } else if (this->player->hasObject(obj)) {
                                        // 120	I ADVISE YOU TO PUT DOWN THE CLAM BEFORE OPENING IT.  >STRAIN!<
                                        return this->data->getMessageByNumber(120)->getContent();
                                    }
                                } else {
                                    // 122	YOU DON'T HAVE ANYTHING STRONG ENOUGH TO OPEN THE CLAM.
                                    return this->data->getMessageByNumber(122)->getContent();
                                }
                            } else if (obj->getNumber() == 1015 /* OYSTER */) {
                                // 1018	KNIFE KNIVE
                                if (this->player->hasObject(this->data->getObjectByNumber(18))) {
                                    if (this->player->getCurrentLocation()->hasObject(obj)) {
                                        // 125	THE OYSTER CREAKS OPEN, REVEALING NOTHING BUT OYSTER INSIDE.  IT
                                        // 125	PROMPTLY SNAPS SHUT AGAIN.
                                        return this->data->getMessageByNumber(125)->getContent();
                                    } else if (this->player->hasObject(obj)) {
                                        // 121	I ADVISE YOU TO PUT DOWN THE OYSTER BEFORE OPENING IT.	>WRENCH!<
                                        return this->data->getMessageByNumber(121)->getContent();
                                    }
                                } else {
                                    // 123	YOU DON'T HAVE ANYTHING STRONG ENOUGH TO OPEN THE OYSTER.
                                    return this->data->getMessageByNumber(123)->getContent();
                                }
                            }
                        break;
                            
                        // 2006 LOCK CLOSE
                        case 2006:
                        
                        break;
                            
                        // 2007 LIGHT ON
                        case 2007:
                            if (this->player->hasObject(obj)) {
                                if (!obj->isLit()) {
                                    obj->setLit(true);
                                    return "You switched the brass lantern on.";
                                } else {
                                    return "This lamp is already lit.";
                                }
                            } else {
                                return "You do not have this object so you can't switch it on.";
                            }
                        break;
                            
                        // 2008 EXTIN OFF
                        case 2008:
                            if (this->player->hasObject(obj)) {
                                if (obj->isLit()) {
                                    obj->setLit(false);
                                    return "You switched the brass lantern off.";
                                } else {
                                    return "This brass lantern is already off.";
                                }
                            } else {
                                return "You do not have this object so you can't switch it off.";
                            }
                        break;
                            
                        // 2009 WAVE SHAKE SWING
                        case 2009:
                            break;
                            
                        // 2010 CALM PLACA TAME
                        case 2010:
                            break;

                        // 2012 ATTAC KILL FIGHT HIT STRIK
                        case 2012:
                            break;
                            
                        // 2014 EAT DEVOU
                        case 2014:
                            if (this->player->hasObject(obj)) {
                                if (obj->getNumber() == 1019 /* FOOD */) {
                                    this->player->removeObject(obj);
                                    return "You just ate: " + obj->getInventoryMessage();
                                } else {
                                    return "You cannot eat this.";
                                }
                            }
                        break;
                            
                        // 2015 DRINK
                        case 2015:
                            obj = this->data->getObjectByNumber(20);
                            // TODO: why is the bottle not found?
                            if (this->player->hasObject(obj)) {
                                // WATER
                                if (obj->getNumber() == 1020) {
                                    if (obj->getPropertyValue() == 0) {
                                        obj->setPropertyValue(1); // 100	THERE IS AN EMPTY BOTTLE HERE.
                                        return "\nYou just drank: " + obj->getInventoryMessage();
                                    } else {
                                        return "The bottle is empty.";
                                    }
                                } else {
                                    return "You cannot drink this.";
                                }
                            } else {
                                return "You are not carrying any bottle to drink from.";
                            }
                        break;
                            
                        // 2016 RUB
                        case 2016:
                            break;
                            
                        // 2017 THROW TOSS
                        case 2017:
                            break;
                            
                        // 2022 FILL
                        case 2022:
                            if (obj->getNumber() == 1020) {
                                if (this->player->hasObject(obj)) {
                                    if (this->player->getCurrentLocation()->isAsset(2) && /* LIQUID ASSET */
                                        !this->player->getCurrentLocation()->isAsset(1) /* OFF FOR WATER */) {
                                        if (obj->getPropertyValue() == 1 /* 1 == empty bottle */) {
                                            obj->setPropertyValue(0); // Bottle is full with water
                                            return "The bottle is now full of water.";
                                        } else {
                                            return "The bottle is full already.";
                                        }
                                    } else {
                                        return "There is no water to fill the bottle with at this location.";
                                    }
                                } else {
                                    return "You are not carrying a bottle.";
                                }
                            } else {
                                return "You cannot fill this object.";
                            }
                        break;
                            
                        default:
                            return verb->getDefaultMessage();
                        break;
                    }
                    
                }
            }
        }
    }
    
    return "";
}