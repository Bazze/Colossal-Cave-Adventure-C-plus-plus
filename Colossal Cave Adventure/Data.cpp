//
//  Data.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include "Data.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>

// Location
#include "Location.h"

// Words
#include "Word.h"
#include "MotionVerb.h"
#include "Object.h"
#include "ActionVerb.h"
#include "SpecialCaseVerb.h"

// Messages
#include "Message.h"
#include "ClassMessage.h"
#include "MagicMessage.h"

// Hint
#include "Hint.h"

// LocationCondition
#include "LocationCondition.h"

Data::Data(string filename) {
    this->locations = new vector<Location*>();
    this->locationConditions = new vector<LocationCondition*>();
    this->words = new vector<Word*>();
    this->messages = new vector<Message*>();
    this->magicMessages = new vector<MagicMessage*>();
    this->classMessages = new vector<ClassMessage*>();
    this->hints = new vector<Hint*>();
    
    this->loadData(filename);
}

Data::~Data() {
    this->deallocLocations();
    this->deallocLocationConditions();
    this->deallocWords();
    this->deallocMessages();
    this->deallocMagicMessages();
    this->deallocClassMessages();
    this->deallocHints();
}

void Data::deallocLocations() {
    for (int i = 0; i < this->locations->size(); i++) {
        delete this->locations->at(i);
    }
    delete this->locations;
}
void Data::deallocLocationConditions() {
    for (int i = 0; i < this->locationConditions->size(); i++) {
        delete this->locationConditions->at(i);
    }
    delete this->locationConditions;
}
void Data::deallocWords() {
    for (int i = 0; i < this->words->size(); i++) {
        delete this->words->at(i);
    }
    delete this->words;
}
void Data::deallocMessages() {
    for (int i = 0; i < this->messages->size(); i++) {
        delete this->messages->at(i);
    }
    delete this->messages;
}
void Data::deallocMagicMessages() {
    for (int i = 0; i < this->magicMessages->size(); i++) {
        delete this->magicMessages->at(i);
    }
    delete this->magicMessages;
}
void Data::deallocClassMessages() {
    for (int i = 0; i < this->classMessages->size(); i++) {
        delete this->classMessages->at(i);
    }
    delete this->classMessages;
}
void Data::deallocHints() {
    for (int i = 0; i < this->hints->size(); i++) {
        delete this->hints->at(i);
    }
    delete this->hints;
}

void Data::loadData(const string filename) {
    // Declare and open file stream
    ifstream dataFile;
    
    dataFile.open(filename, ios::in);
    
    // If the file was successfully opened, start processing
    if (dataFile.is_open()) {

        // Parse the lines in the file
        this->parseLines(dataFile);

        // Close file stream
        dataFile.close();
    }
    //this->dumpAllLocations();
    //this->dumpAllWords();
    //this->dumpAllMessages();
    //this->dumpAllClassMessages();
    //this->dumpAllHints();
    //this->dumpAllMagicMessages();
}

void Data::parseLines(ifstream &dataFile) {
    string line;
    vector<string> lineVector;

    // The data file is divided into different sections, this
    // variable keeps track of which section is being parsed
    int currentSection = -1;

    // Since a location's information can be stored on several
    // lines we need to keep track of the current location
    // number being processed
    //int currentLocation = -1;

    Location* currentLocation = NULL;
    Location* accessibleLocation = NULL;
    Word* currentWord = NULL;
    MotionVerb* currentMotionVerb = NULL;
    Object* currentObject = NULL;
    Message* currentMessage = NULL;
    ActionVerb* currentActionVerb = NULL;

    int idNumber = -1;

    // Run as long as there are lines left to read
    while (dataFile.good()) {
        // Get line from the file stream
        getline(dataFile, line);

        // Clear the vector from previous lines
        if (lineVector.size() > 0) {
            lineVector.clear();
        }

        // Parse the line
        this->split(line, '\t', lineVector);

        // If this line is the end of the section, proceed to next section
        if (lineVector.size() > 0 && lineVector.at(0) == "-1") {
            // Reset some variables for the next section
            currentSection = -1;
            idNumber = -1;
            currentLocation = NULL;
            accessibleLocation = NULL;
            currentWord = NULL;
            currentMotionVerb = NULL;
            currentObject = NULL;
            currentMessage = NULL;
            currentActionVerb = NULL;

            continue;
        }
        // If it's not the end of a section, check if it is the beginning
        else if (currentSection == -1) {
            currentSection = atoi(lineVector.at(0).c_str());

            // We don't want to parse the line which indicate the beginning
            // of a section
            continue;
        }
        else if (currentSection != 0) {
            // First is always an id/number
            idNumber = atoi(lineVector.at(0).c_str());
        }

        // Handle the data in different ways depending on the currentSection
        switch (currentSection) {

            /* ------------------------------------------------------------------
             * SECTION 1:
             * ------------------------------------------------------------------
             C LONG FORM DESCRIPTIONS. EACH LINE CONTAINS A LOCATION NUMBER,
             C A TAB, AND A LINE OF TEXT. THE SET OF (NECESSARILY ADJACENT) LINES
             C WHOSE NUMBERS ARE X FORM THE LONG DESCRIPTION OF LOCATION X.
             */
            case 1:
            {
                if (currentLocation == NULL || idNumber != currentLocation->getNumber()) {
                    currentLocation = new Location(idNumber);
                    this->locations->push_back(currentLocation);
                }
                currentLocation->appendToLongDescription(lineVector.at(1));
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 2:
             * ------------------------------------------------------------------
             C SHORT FORM DESCRIPTIONS.  SAME FORMAT AS LONG FORM. NOT ALL
             C PLACES HAVE SHORT DESCRIPTIONS.
             */
            case 2:
            {
                if (currentLocation == NULL || idNumber != currentLocation->getNumber()) {
                    currentLocation = this->getLocationByNumber(idNumber);
                }
                currentLocation->appendToShortDescription(lineVector.at(1));
            }
            break;


            /* ------------------------------------------------------------------
             * SECTION 3:
             * ------------------------------------------------------------------
             C TRAVEL TABLE.  EACH LINE CONTAINS A LOCATION NUMBER (X), A SECOND
             C	LOCATION NUMBER (Y), AND A LIST OF MOTION NUMBERS (SEE SECTION 4).
             C	EACH MOTION REPRESENTS A VERB WHICH WILL GO TO Y IF CURRENTLY AT X.
             C	Y, IN TURN, IS INTERPRETED AS FOLLOWS.  LET M=Y/1000, N=Y MOD 1000.
             C		IF N<=300	IT IS THE LOCATION TO GO TO.
             C		IF 300<N<=500	N-300 IS USED IN A COMPUTED GOTO TO
             C					A SECTION OF SPECIAL CODE.
             C		IF N>500	MESSAGE N-500 FROM SECTION 6 IS PRINTED,
             C					AND HE STAYS WHEREVER HE IS.
             C	MEANWHILE, M SPECIFIES THE CONDITIONS ON THE MOTION.
             C		IF M=0		IT'S UNCONDITIONAL.
             C		IF 0<M<100	IT IS DONE WITH M% PROBABILITY.
             C		IF M=100	UNCONDITIONAL, BUT FORBIDDEN TO DWARVES.
             C		IF 100<M<=200	HE MUST BE CARRYING OBJECT M-100.
             C		IF 200<M<=300	MUST BE CARRYING OR IN SAME ROOM AS M-200.
             C		IF 300<M<=400	PROP(M MOD 100) MUST *NOT* BE 0.
             C		IF 400<M<=500	PROP(M MOD 100) MUST *NOT* BE 1.
             C		IF 500<M<=600	PROP(M MOD 100) MUST *NOT* BE 2, ETC.
             C	IF THE CONDITION (IF ANY) IS NOT MET, THEN THE NEXT *DIFFERENT*
             C	"DESTINATION" VALUE IS USED (UNLESS IT FAILS TO MEET *ITS* CONDITIONS,
             C	IN WHICH CASE THE NEXT IS FOUND, ETC.).  TYPICALLY, THE NEXT DEST WILL
             C	BE FOR ONE OF THE SAME VERBS, SO THAT ITS ONLY USE IS AS THE ALTERNATE
             C	DESTINATION FOR THOSE VERBS.  FOR INSTANCE:
             C		15	110022	29	31	34	35	23	43
             C		15	14	29
             C	THIS SAYS THAT, FROM LOC 15, ANY OF THE VERBS 29, 31, ETC., WILL TAKE
             C	HIM TO 22 IF HE'S CARRYING OBJECT 10, AND OTHERWISE WILL GO TO 14.
             C		11	303008	49
             C		11	9	50
             C	THIS SAYS THAT, FROM 11, 49 TAKES HIM TO 8 UNLESS PROP(3)=0, IN WHICH
             C	CASE HE GOES TO 9.  VERB 50 TAKES HIM TO 9 REGARDLESS OF PROP(3).
             */
            case 3:
            {
                bool newLoc = false;
                int Y = atoi(lineVector.at(1).c_str());
                int N = Y%1000;
                
                if (currentLocation == NULL || idNumber != currentLocation->getNumber()) {
                    newLoc = true;
                    currentLocation = this->getLocationByNumber(idNumber);
                    if (currentLocation == NULL) {
                        // For debugging
                        cout << "ERROR: Section 3.1: Location not found: " << idNumber << endl;
                    }
                }
                
                // TODO: N==0? What to do? What does it mean?
                // IF N<=300	IT IS THE LOCATION TO GO TO.
                if (N > 0 && N < 300) {
                    if (accessibleLocation == NULL || N != accessibleLocation->getNumber() || newLoc) {
                        accessibleLocation = this->getLocationByNumber(N);
                        
                        // For debugging
                        if (accessibleLocation == NULL) {
                            cout << "ERROR: Section 3.2: Location not found: " << N << endl;
                        }
                        
                        // Add the accessible Location
                        currentLocation->addAccessibleLocation(accessibleLocation);
                        
                        // Add the condition for going to this location
                        LocationCondition* cond = new LocationCondition(Y/1000, currentLocation, accessibleLocation);
                        this->locationConditions->push_back(cond);
                        currentLocation->addLocationCondition(cond);
                        
                        // Add the possible MotionVerbs which can be used to go to this location
                        int verbNr = -1;
                        for (int i = 2; i < lineVector.size(); i++) {
                            verbNr = atoi(lineVector.at(i).c_str());
                            currentMotionVerb = this->getMotionVerbByNumber(verbNr);
                            
                            if (currentMotionVerb == NULL) {
                                currentMotionVerb = new MotionVerb(verbNr);
                                this->words->push_back(currentMotionVerb);
                            }
                            
                            currentLocation->addMotionVerb(accessibleLocation, currentMotionVerb);
                        }
                    }
                }
                // IF N>500	MESSAGE N-500 FROM SECTION 6 IS PRINTED, AND HE STAYS WHEREVER HE IS.
                else if (N > 500) {
                    if (currentMessage == NULL || N != currentMessage->getNumber() || newLoc) {
                        currentMessage = this->getMessageByNumber(N-500);
                        if (currentMessage == NULL) {
                            currentMessage = new Message(N-500);
                            this->messages->push_back(currentMessage);
                        }
                        
                        currentLocation->addPrintMessage(currentMessage);
                        
                        // Add the possible MotionVerbs which can be used to go to this location
                        int verbNr = -1;
                        for (int i = 2; i < lineVector.size(); i++) {
                            verbNr = atoi(lineVector.at(i).c_str());
                            currentMotionVerb = this->getMotionVerbByNumber(verbNr);
                            
                            if (currentMotionVerb == NULL) {
                                currentMotionVerb = new MotionVerb(verbNr);
                                this->words->push_back(currentMotionVerb);
                            }
                            
                            currentLocation->addMotionVerbForPrintMessage(currentMessage, currentMotionVerb);
                        }
                    }
                }
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 4:
             * ------------------------------------------------------------------
             C VOCABULARY.  EACH LINE CONTAINS A NUMBER (N), A TAB, AND A
             C	FIVE-LETTER WORD.  CALL M=N/1000.  IF M=0, THEN THE WORD IS A MOTION
             C	VERB FOR USE IN TRAVELLING (SEE SECTION 3).  ELSE, IF M=1, THE WORD IS
             C	AN OBJECT.  ELSE, IF M=2, THE WORD IS AN ACTION VERB (SUCH AS "CARRY"
             C	OR "ATTACK").  ELSE, IF M=3, THE WORD IS A SPECIAL CASE VERB (SUCH AS
             C	"DIG") AND N MOD 1000 IS AN INDEX INTO SECTION 6.  OBJECTS FROM 50 TO
             C	(CURRENTLY, ANYWAY) 79 ARE CONSIDERED TREASURES (FOR PIRATE, CLOSEOUT).
             */
            case 4:
            {
                int M = idNumber/1000;

                if (currentWord == NULL || idNumber != currentWord->getNumber()) {
                    bool push = true;
                    // Check what type of word it is and create an appropriate object
                    if (M == 0) {
                        // Since some MotionVerbs were created in the previous section,
                        // we need to make sure grab the old one and don't create a duplicate
                        currentWord = this->getMotionVerbByNumber(idNumber);
                        if (currentWord == NULL) {
                            currentWord = new MotionVerb(idNumber, lineVector.at(1));
                        } else {
                            currentWord->addWord(lineVector.at(1));
                            push = false;
                        }
                    } else if (M == 1) {
                        currentWord = new Object(idNumber, lineVector.at(1));
                        // OBJECTS FROM 50 TO 79 ARE CONSIDERED TREASURES
                        if (idNumber%1000 >= 50 && idNumber%1000 <= 79) {
                            ((Object*)currentWord)->setTreasure(true);
                        } else if (idNumber%1000 == 2) {
                            ((Object*)currentWord)->setLightable(true);
                        }
                    } else if (M == 2) {
                        currentWord = new ActionVerb(idNumber, lineVector.at(1));
                    } else if (M == 3) {
                        currentWord = new SpecialCaseVerb(idNumber, lineVector.at(1));
                    }

                    // Add the word to our vector if needed (not needed for MotionVerbs already existing)
                    if (push) {
                        this->words->push_back(currentWord);
                    }
                } else {
                    currentWord->addWord(lineVector.at(1));
                }

                // If a comment is supplied, add it to the word
                if (lineVector.size() > 2) {
                    currentWord->setComment(lineVector.at(2));
                }
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 5:
             * ------------------------------------------------------------------
             C OBJECT DESCRIPTIONS.  EACH LINE CONTAINS A NUMBER (N), A TAB,
             C	AND A MESSAGE.  IF N IS FROM 1 TO 100, THE MESSAGE IS THE "INVENTORY"
             C	MESSAGE FOR OBJECT N.  OTHERWISE, N SHOULD BE 000, 100, 200, ETC., AND
             C	THE MESSAGE SHOULD BE THE DESCRIPTION OF THE PRECEDING OBJECT WHEN ITS
             C	PROP VALUE IS N/100.  THE N/100 IS USED ONLY TO DISTINGUISH MULTIPLE
             C	MESSAGES FROM MULTI-LINE MESSAGES; THE PROP INFO ACTUALLY REQUIRES ALL
             C	MESSAGES FOR AN OBJECT TO BE PRESENT AND CONSECUTIVE.  PROPERTIES WHICH
             C	PRODUCE NO MESSAGE SHOULD BE GIVEN THE MESSAGE ">$<".
            */
            case 5:
            {
                if (currentObject == NULL || (idNumber >= 1 && idNumber < 100)) {
                    currentObject = this->getObjectByNumber(idNumber);
                    // For debugging
                    if (currentObject == NULL) {
                        cout << idNumber << ": Object not found" << endl;
                    } else {
                        currentObject->setInventoryMessage(lineVector.at(1));
                    }
                } else {
                    // If this property description is another line from the previous one - append,
                    // otherwise add as a new property description
                    if (idNumber == currentObject->getNumber()) {
                        currentObject->appendToPropertyDescription(idNumber, lineVector.at(1));
                    } else {
                        currentObject->addPropertyDescription(lineVector.at(1));
                    }
                }
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 6:
             * ------------------------------------------------------------------
             C  ARBITRARY MESSAGES.  SAME FORMAT AS SECTIONS 1, 2, AND 5, EXCEPT
             C	THE NUMBERS BEAR NO RELATION TO ANYTHING (EXCEPT FOR SPECIAL VERBS)
             */
            case 6:
            {
                if (currentMessage == NULL || idNumber != currentMessage->getNumber()) {
                    currentMessage = this->getMessageByNumber(idNumber);
                    
                    if (currentMessage == NULL) {
                        currentMessage = new Message(idNumber);
                        this->messages->push_back(currentMessage);
                    }
                }
                currentMessage->appendContent(lineVector.at(1));
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 7:
             * ------------------------------------------------------------------
             C  OBJECT LOCATIONS.  EACH LINE CONTAINS AN OBJECT NUMBER AND ITS
             C	INITIAL LOCATION (ZERO (OR OMITTED) IF NONE).  IF THE OBJECT IS
             C	IMMOVABLE, THE LOCATION IS FOLLOWED BY A "-1".  IF IT HAS TWO LOCATIONS
             C	(E.G. THE GRATE) THE FIRST LOCATION IS FOLLOWED WITH THE SECOND, AND
             C	THE OBJECT IS ASSUMED TO BE IMMOVABLE.
             */
            case 7:
            {
                int locNr = (lineVector.size() > 1 ? atoi(lineVector.at(1).c_str()) : 0);
                // If second argument on the line is omitted or zero, the object has no initial location
                if (locNr != 0) {
                    currentObject = this->getObjectByNumber(idNumber);
                    currentLocation = this->getLocationByNumber(locNr);
                    if (currentObject != NULL && currentLocation != NULL) {
                        currentLocation->addObject(currentObject);
                        // Does this object have a second location? Or is it immovable?
                        if (lineVector.size() > 2) {
                            locNr = atoi(lineVector.at(2).c_str());
                            if (locNr != -1) {
                                currentLocation = this->getLocationByNumber(locNr);
                                if (currentLocation != NULL) {
                                    currentLocation->addObject(currentObject);
                                } else {
                                    cout << "ERROR: Section 7.1: Could not find location" << endl;
                                }
                            }
                            currentObject->setMovable(false);
                        }
                    } else {
                        // For debugging
                        cout << "ERROR: Section 7.2: Could not find location" << endl;
                    }
                }
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 8:
             * ------------------------------------------------------------------
             C ACTION DEFAULTS.  EACH LINE CONTAINS AN "ACTION-VERB" NUMBER AND
             C	THE INDEX (IN SECTION 6) OF THE DEFAULT MESSAGE FOR THE VERB.
             */
            case 8:
            {
                if (currentActionVerb == NULL || idNumber != currentActionVerb->getNumber()) {
                    currentActionVerb = this->getActionVerbByNumber(idNumber);
                    // For debugging
                    if (currentActionVerb == NULL) {
                        cout << "ERROR: Section 8.1: Could not find ActionVerb" << endl;
                    }
                }
                currentMessage = this->getMessageByNumber(atoi(lineVector.at(1).c_str()));
                // Some action verbs doesn't have a default message (0)
                if (currentMessage != NULL) {
                    currentActionVerb->setDefaultMessage(currentMessage);
                }

            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 9:
             * ------------------------------------------------------------------
             C LIQUID ASSETS, ETC.  EACH LINE CONTAINS A NUMBER (N) AND UP TO 20
             C	LOCATION NUMBERS.  BIT N (WHERE 0 IS THE UNITS BIT) IS SET IN COND(LOC)
             C	FOR EACH LOC GIVEN.  THE COND BITS CURRENTLY ASSIGNED ARE:
             C		0	LIGHT
             C		1	IF BIT 2 IS ON: ON FOR OIL, OFF FOR WATER
             C		2	LIQUID ASSET, SEE BIT 1
             C		3	PIRATE DOESN'T GO HERE UNLESS FOLLOWING PLAYER
             C	OTHER BITS ARE USED TO INDICATE AREAS OF INTEREST TO "HINT" ROUTINES:
             C		4	TRYING TO GET INTO CAVE
             C		5	TRYING TO CATCH BIRD
             C		6	TRYING TO DEAL WITH SNAKE
             C		7	LOST IN MAZE
             C		8	PONDERING DARK ROOM
             C		9	AT WITT'S END
             C	COND(LOC) IS SET TO 2, OVERRIDING ALL OTHER BITS, IF LOC HAS FORCED
             C	MOTION.
             */
            case 9:
            {
                for (int i = 1; i < lineVector.size(); i++) {
                    currentLocation = this->getLocationByNumber(atoi(lineVector.at(i).c_str()));
                    
                    // For debugging
                    if (currentLocation == NULL) {
                        cout << "ERROR: Section 9.1: Could not find location" << endl;
                    }
                    
                    currentLocation->setAsset(idNumber, true);
                }
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 10:
             * ------------------------------------------------------------------
             C CLASS MESSAGES.  EACH LINE CONTAINS A NUMBER (N), A TAB, AND A
             C	MESSAGE DESCRIBING A CLASSIFICATION OF PLAYER.  THE SCORING SECTION
             C	SELECTS THE APPROPRIATE MESSAGE, WHERE EACH MESSAGE IS CONSIDERED TO
             C	APPLY TO PLAYERS WHOSE SCORES ARE HIGHER THAN THE PREVIOUS N BUT NOT
             C	HIGHER THAN THIS N.  NOTE THAT THESE SCORES PROBABLY CHANGE WITH EVERY
             C	MODIFICATION (AND PARTICULARLY EXPANSION) OF THE PROGRAM.
             */
            case 10:
            {
                this->classMessages->push_back(new ClassMessage(idNumber, lineVector.at(1)));
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 11:
             * ------------------------------------------------------------------
             C HINTS.  EACH LINE CONTAINS A
                - HINT NUMBER (CORRESPONDING TO A COND BIT, SEE SECTION 9)
                - THE NUMBER OF TURNS HE MUST BE AT THE RIGHT LOC(S) BEFORE TRIGGERING THE HINT
                - THE POINTS DEDUCTED FOR TAKING THE HINT
                - THE MESSAGE NUMBER (SECTION 6) OF THE QUESTION
                - AND THE MESSAGE NUMBER OF THE HINT.
             C  THESE VALUES ARE STASHED IN THE "HINTS" ARRAY.
             C	HNTMAX IS SET TO THE MAX HINT NUMBER (<= HNTSIZ).  NUMBERS 1-3 ARE
             C	UNUSABLE SINCE COND BITS ARE OTHERWISE ASSIGNED, SO 2 IS USED TO
             C	REMEMBER IF HE'S READ THE CLUE IN THE REPOSITORY, AND 3 IS USED TO
             C	REMEMBER WHETHER HE ASKED FOR INSTRUCTIONS (GETS MORE TURNS, BUT LOSES
             C	POINTS).
             */
            case 11:
            {
                Message* question = this->getMessageByNumber(atoi(lineVector.at(3).c_str()));
                Message* hint = this->getMessageByNumber(atoi(lineVector.at(4).c_str()));

                // For debugging
                if ((atoi(lineVector.at(3).c_str()) != 0 && question == NULL) || (atoi(lineVector.at(4).c_str()) != 0 && hint == NULL)) {
                    cout << "ERROR: Section 11.1: Could not find question or hint message." << endl;
                }

                int assetIndex = atoi(lineVector.at(0).c_str());
                Hint* currentHint = new Hint(
                    assetIndex,
                    atoi(lineVector.at(1).c_str()),
                    atoi(lineVector.at(2).c_str()),
                    question,
                    hint
                );
                this->hints->push_back(currentHint);
                
                for (int i = 0; i < this->locations->size(); i++) {
                    if (this->locations->at(i)->isAsset(assetIndex)) {
                        this->locations->at(i)->addHint(currentHint);
                    }
                }
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 12:
             * ------------------------------------------------------------------
             C MAGIC MESSAGES. IDENTICAL TO SECTION 6 EXCEPT PUT IN A SEPARATE
             C	SECTION FOR EASIER REFERENCE.  MAGIC MESSAGES ARE USED BY THE STARTUP,
             C	MAINTENANCE MODE, AND RELATED ROUTINES.
             */
            case 12:
            {
                if (currentMessage == NULL || idNumber != currentMessage->getNumber()) {
                    currentMessage = new MagicMessage(idNumber);
                    this->magicMessages->push_back((MagicMessage*)currentMessage);
                }
                currentMessage->appendContent(lineVector.at(1));
            }
            break;

            /* ------------------------------------------------------------------
             * SECTION 0:
             * ------------------------------------------------------------------
             C END OF DATABASE.
             */
            case 0:
                return;
            break;
        }
    }

}

Location* Data::getLocationByNumber(const int n) {
    Location *l;
    for (int i = 0; i < this->locations->size(); i++) {
        l = this->locations->at(i);
        if (l->getNumber() == n) {
            return l;
        }
    }
    return NULL;
}
Object* Data::getObjectByNumber(const int n) {
    for (int i = 0; i < this->words->size(); i++) {
        if (dynamic_cast<Object*>(this->words->at(i)) != NULL) {
            Object *o = (Object*)this->words->at(i);
            if (o->getNumber()%1000 == n) {
                return o;
            }
        }
    }
    return NULL;
}
ActionVerb* Data::getActionVerbByNumber(const int n) {
    for (int i = 0; i < this->words->size(); i++) {
        if (dynamic_cast<ActionVerb*>(this->words->at(i)) != NULL) {
            ActionVerb *o = (ActionVerb*)this->words->at(i);
            if (o->getNumber()%2000 == n) {
                return o;
            }
        }
    }
    return NULL;
}
MotionVerb* Data::getMotionVerbByNumber(const int n) {
    for (int i = 0; i < this->words->size(); i++) {
        if (dynamic_cast<MotionVerb*>(this->words->at(i)) != NULL) {
            MotionVerb* o = (MotionVerb*)this->words->at(i);
            if (o->getNumber() == n) {
                return o;
            }
        }
    }
    return NULL;
}
Message* Data::getMessageByNumber(const int n) {
    Message *m;
    for (int i = 0; i < this->messages->size(); i++) {
        m = this->messages->at(i);
        if (m->getNumber() == n) {
            return m;
        }
    }
    return NULL;
}

LocationCondition* Data::getLocationCondition(Location* from, Location* to) {
    
}

/* Split function from: http://stackoverflow.com/questions/236129/splitting-a-string-in-c#answer-236803 */
vector<string>& Data::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
/* End split */

/*
const string Data::trim(const string& pString, const string& pWhitespace) {
    const size_t beginStr = pString.find_first_not_of(pWhitespace);
    if (beginStr == string::npos)
    {
        // no content
        return "";
    }

    const size_t endStr = pString.find_last_not_of(pWhitespace);
    const size_t range = endStr - beginStr + 1;

    return pString.substr(beginStr, range);
}

const string Data::reduce(const string& pString, const string& pFill, const string& pWhitespace) {
    // trim first
    string result(trim(pString, pWhitespace));

    // replace sub ranges
    size_t beginSpace = result.find_first_of(pWhitespace);
    while (beginSpace != string::npos) {
        const size_t endSpace = result.find_first_not_of(pWhitespace, beginSpace);
        const size_t range = endSpace - beginSpace;

        result.replace(beginSpace, range, pFill);

        const size_t newStart = beginSpace + pFill.length();
        beginSpace = result.find_first_of(pWhitespace, newStart);
    }

    return result;
}*/

Word* Data::findWord(string word) const {
    // Find the word
    for (int i = 0; i < this->words->size(); i++) {
        if (this->words->at(i)->hasWord(word)) {
            // Return a pointer to the word
            return this->words->at(i);
        }
    }
    // If no word was found, return NULL
    return NULL;
}

void Data::dumpAllLocations() {
    for (int i = 0; i < this->locations->size(); i++) {
        cout << this->locations->at(i)->toString() << endl;
    }
}
void Data::dumpAllWords() {
    for (int i = 0; i < this->words->size(); i++) {
        cout << this->words->at(i)->toString() << endl;
    }
}
void Data::dumpAllMessages() {
    for (int i = 0; i < this->messages->size(); i++) {
        cout << this->messages->at(i)->toString() << endl;
    }
}
void Data::dumpAllMagicMessages() {
    for (int i = 0; i < this->magicMessages->size(); i++) {
        cout << this->magicMessages->at(i)->toString() << endl;
    }
}
void Data::dumpAllClassMessages() {
    for (int i = 0; i < this->classMessages->size(); i++) {
        cout << this->classMessages->at(i)->toString() << endl;
    }
}
void Data::dumpAllHints() {
    for (int i = 0; i < this->hints->size(); i++) {
        cout << this->hints->at(i)->toString() << endl;
    }
}