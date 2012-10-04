//
//  Word.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-09-29.
//  Copyright (c) 2012. All rights reserved.
//

#include <sstream>

#include "Word.h"


Word::Word() {
    this->number = 0;
    this->words = new vector<string>();
    this->comment = "";
}
Word::Word(int number, string word, string comment) {
    this->number = number;
    this->words = new vector<string>();
    if (word != "") {
        this->words->push_back(word);
    }
    this->comment = comment;
}
Word::~Word() {
    delete this->words;
}

/*Word::Word(const Word& source) {
    cout << "called" << endl;
    this->number = source.number;
    this->words = new vector<string>();
    for (int i = 0; i < source.words->size(); i++) {
        this->words->push_back(source.words->at(i));
    }
    this->comment = source.comment;
}*/

const int Word::getNumber() const {
    return this->number;
}
void Word::setNumber(int number) {
    this->number = number;
}

string Word::getWords() {
    string s = "";
    for (int i = 0; i < this->words->size(); i++) {
        s += this->words->at(i);
        if (i != this->words->size()-1) {
            s += " ";
        }
    }
    return s;
}
void Word::addWord(string word) {
    this->words->push_back(word);
}

string Word::getComment() {
    return this->comment;
}
void Word::setComment(string comment) {
    this->comment = comment;
}

const string Word::showSpec() {
    stringstream s;
    s << "Number: " << this->getNumber() << endl << "Word:" << endl << this->getWords() << endl << "Comment:" << endl << (this->getComment() == "" ? "<empty>" : this->getComment()) << endl;
    return s.str();
}