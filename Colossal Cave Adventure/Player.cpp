//
//  Player.cpp
//  Colossal Cave Adventure
//
//  Created by Sebastian Norling and Niclas Björner on 2012-10-05.
//  Copyright (c) 2012. All rights reserved.
//

#include "Player.h"

Player::Player(Location* initialLocation) {
    this->score = 0;
    this->currentLocation = initialLocation;
    this->objects = new vector<Object*>();
}
Player::~Player() {
    delete this->objects;
}

int Player::getScore() const {
    return this->score;
}
void Player::addScore(int s) {
    this->score += s;
}
void Player::setScore(int s) {
    this->score = s;
}

void Player::setCurrentLocation(Location *loc) {
    this->currentLocation = loc;
}
Location* Player::getCurrentLocation() const {
    return this->currentLocation;
}

void Player::pickUpObject(Object* obj) {
    this->objects->push_back(obj);
}
void Player::dropObject(Object *obj) {
    bool found = false;
    for (int i = 0; i < this->objects->size() && !found; i++) {
        // OK to compare pointers
        if (this->objects->at(i) == obj) {
            this->objects->erase(this->objects->begin()+i);
            found = true;
        }
    }
}
bool Player::hasObject(Object *obj) const {
    bool found = false;
    for (int i = 0; i < this->objects->size() && !found; i++) {
        // OK to compare pointers
        if (this->objects->at(i) == obj) {
            found = true;
        }
    }
    return found;
}
vector<Object*>* Player::getObjects() const {
    return this->objects;
}