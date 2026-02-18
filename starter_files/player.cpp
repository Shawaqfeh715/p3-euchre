#include "Player.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

// Simpler player implementation
class SimplePlayer : public Player {
public:
SimplePlayer(const string &name) : name(name) {}

const string & get_name() const override {
    return name;
}

void add_card(const Card &c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
  }


}