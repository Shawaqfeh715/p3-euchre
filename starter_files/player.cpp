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
  
  // FIXED: Renamed from add_and_remove to add_and_discard to match Player.hpp
  void add_and_discard(const Card &upcard) override {
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

  Card lead_card(Suit trump) override {
    auto best_it = hand.begin();
    bool has_a_non_trump = false;

    // first, let's try to find highest non trump
    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (!it->is_trump(trump)) {
        if(!has_a_non_trump || Card_less(*best_it, *it, trump)) {
          best_it = it;
          has_a_non_trump = true;
        }
      }
    } // FIXED: Moved brace here so it properly ends the non-trump search

    // If only trump, find the highest trump
    if (!has_a_non_trump) { // FIXED: Variable name matched your 'has_a_non_trump'
      best_it = hand.begin(); // Reset to start for second pass
      for (auto it = hand.begin(); it != hand.end(); ++it) {
        if (Card_less(*best_it, *it, trump)) {
          best_it = it;
        }
      }
    }

    Card played = *best_it;
    hand.erase(best_it);
    return played;
  }

  Card play_card(const Card &led_card, Suit trump) override {
    Suit led_suit = led_card.get_suit(trump);
    auto best_it = hand.begin();
    bool can_follow_suit = false;

    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (it->get_suit(trump) == led_suit) {
        // Highest card that follows suit
        if (!can_follow_suit || Card_less(*best_it, *it, led_card, trump)) {
          best_it = it;
          can_follow_suit = true;
        }
      }
    }

    if (!can_follow_suit) {
      // Lowest card overall
      best_it = hand.begin(); // Reset iterator to look through everything
      for (auto it = hand.begin(); it != hand.end(); ++it) {
        if (Card_less(*it, *best_it, led_card, trump)) {
          best_it = it;
        }
      }
    }

    Card played = *best_it;
    hand.erase(best_it);
    return played;
  }

  private : 
  string name;
  vector<Card> hand;

};

class HumanPlayer : public Player {
  public:
  HumanPlayer(const string &name) : name(name) {}

  const string & get_name() const override { return name; }

  // Logic: adding card, then sorting 
  void add_card(const Card &c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
    sort(hand.begin(), hand.end());

    bool make_trump (const Card &upcard, bool is_dealer, int round, Suit & order_up_suit) const override {
      print_hand();
      cout << "Human Player " << name << ", please enter a suit, or \"pass\":\n";

      string decision;
      cin >> decision;


      if (decision != "pass") {
        order_up_suit = string_to_suit(decision);
        return true;
      }
      return false;
    }

    void add_and_discard(const Card &upcard) override {
      // adding upcard to choose from the 6 cards
      hand.push_back(upcard);
      sort(hand.begin(), hand.end());

      print_hand();
      cout << "Discard upcard: [-1]\n";
      cout << "Human player " << name << ", please select a card to discard:\n";

      int choice;
      cin >> choice;

      if (choice == -1) {
        // discard upcard
        auto it = find(hand.begin(), hand.end(), upcard);
      hand.erase(it);
    } else {
      hand.erase(hand.begin() + choice);
    }
    // sorting again to be sure
    sort(hand.begin(), hand.end());
  }

  Card lead_card(Suit trump) override {
    sort(hand.begin(), hand.end()); 
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";
  }

  int choice;
    cin >> choice;
    Card played = hand[choice];
    hand.erase(hand.begin() + choice);
    return played;
  }

  Card play_card(const Card &led_card, Suit trump) override {
    // For a Human, leading and playing look the same: pick a card
    return lead_card(trump); 
  }

private:
  string name;
  vector<Card> hand;

  // Helper function required by specs
  void print_hand() const {
    for (size_t i = 0; i < hand.size(); ++i) {
      cout << "Human player " << name << "'s hand: [" 
           << i << "] " << hand[i] << "\n";
    }
  }
};


