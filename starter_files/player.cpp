#include "Player.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

class SimplePlayer : public Player {
public:
  SimplePlayer(const string &name) : name(name) {}

  const string & get_name() const override { return name; }

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
        if (c.is_trump(candidate) && c.is_face_or_ace()) { count++; }
      }
      if (count >= 2) { order_up_suit = candidate; return true; }
    } 
    else if (round == 2) {
      candidate = Suit_next(upcard.get_suit());
      if (is_dealer) { order_up_suit = candidate; return true; }
      for (const auto &c : hand) {
        if (c.is_trump(candidate) && c.is_face_or_ace()) { count++; }
      }
      if (count >= 1) { order_up_suit = candidate; return true; }
    }
    return false;
  }
  
  void add_and_discard(const Card &upcard) override {
    assert(hand.size() >= 1);
    hand.push_back(upcard);
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
    for (auto it = hand.begin(); it != hand.end(); ++it) {
      if (!it->is_trump(trump)) {
        if(!has_a_non_trump || Card_less(*best_it, *it, trump)) {
          best_it = it;
          has_a_non_trump = true;
        }
      }
    }
    if (!has_a_non_trump) {
      best_it = hand.begin();
      for (auto it = hand.begin(); it != hand.end(); ++it) {
        if (Card_less(*best_it, *it, trump)) { best_it = it; }
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
        if (!can_follow_suit || Card_less(*best_it, *it, led_card, trump)) {
          best_it = it;
          can_follow_suit = true;
        }
      }
    }
    if (!can_follow_suit) {
      best_it = hand.begin(); 
      for (auto it = hand.begin(); it != hand.end(); ++it) {
        if (Card_less(*it, *best_it, led_card, trump)) { best_it = it; }
      }
    }
    Card played = *best_it;
    hand.erase(best_it);
    return played;
  }

private: 
  string name;
  vector<Card> hand;
};

class HumanPlayer : public Player {
public:
  HumanPlayer(const string &name) : name(name) {}
  const string & get_name() const override { return name; }
  void add_card(const Card &c) override {
    hand.push_back(c);
    sort(hand.begin(), hand.end());
  }
  bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override {
    print_hand();
    cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
    string decision;
    cin >> decision;
    if (decision != "pass") { order_up_suit = string_to_suit(decision); return true; }
    return false;
  }
  void add_and_discard(const Card &upcard) override {
    hand.push_back(upcard);
    sort(hand.begin(), hand.end());
    print_hand();
    cout << "Discard upcard: [-1]\n";
    cout << "Human player " << name << ", please select a card to discard:\n";
    int choice;
    cin >> choice;
    if (choice == -1) {
      auto it = find(hand.begin(), hand.end(), upcard);
      hand.erase(it);
    } else { hand.erase(hand.begin() + choice); }
    sort(hand.begin(), hand.end());
  }
  Card lead_card(Suit trump) override {
    print_hand();
    cout << "Human player " << name << ", please select a card:\n";
    int choice;
    cin >> choice;
    Card played = hand[choice];
    hand.erase(hand.begin() + choice);
    return played;
  }
  Card play_card(const Card &led_card, Suit trump) override { return lead_card(trump); }
private:
  string name;
  vector<Card> hand;
  void print_hand() const {
    for (size_t i = 0; i < hand.size(); ++i) {
      cout << "Human player " << name << "'s hand: [" << i << "] " << hand[i] << "\n";
    }
  }
};

Player * Player_factory(const string &name, const string &strategy) {
  if (strategy == "Simple") { return new SimplePlayer(name); }
  if (strategy == "Human") { return new HumanPlayer(name); }
  assert(false); return nullptr;
}
ostream & operator<<(ostream &os, const Player &p) { os << p.get_name(); return os; }