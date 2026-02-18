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


  bool make_trump(const Card &upcard, bool is_dealer,
                  int round, Suit &order_up_suit) const override {
    int count = 0;
    Suit candidate;

    if (round == 1) {
      candidate = upcard.get_suit();
      for (const auto &c : hand) {
        if (c.is_trump(candidate) && c.is_face_or_ace()) {
          count++;
        }
      }
      if (count >= 2) {
        order_up_suit = candidate;
        return true;
      }
    } 
    else if (round == 2) {
      candidate = Suit_next(upcard.get_suit());
      if (is_dealer) { // Screw the dealer
        order_up_suit = candidate;
        return true;
      }
      for (const auto &c : hand) {
        if (c.is_trump(candidate) && c.is_face_or_ace()) {
          count++;
        }
      }
      if (count >= 1) {
        order_up_suit = candidate;
        return true;
      }
    }
    return false;
  }
  
  void add_and_remove(const Card &upcard) override {
    assert(hand.size() >= 1);
    hand.push_back(upcard);

    // discard the lowest card even if it's upcard 
    auto lowest_it = hand.begin();
    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (Card_less(*it, *lowest_it, upcard.get_suit())) {
        lowest_it = it;
      }
    }
    hand.erase(lowest_it);
  }
};