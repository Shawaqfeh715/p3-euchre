#include "Player.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SimplePlayer : public Player {
public:
  explicit SimplePlayer(const string &name_in) : name(name_in) {}

  const string &get_name() const override { return name; }

  void add_card(const Card &c) override { hand.push_back(c); }

  bool make_trump(const Card &upcard, bool is_dealer, int round,
                  Suit &order_up_suit) const override {
    assert(round == 1 || round == 2);
    const Suit upcard_suit = upcard.get_suit();

    if (round == 1) {
      int trump_face_aces = 0;
      for (size_t i = 0; i < hand.size(); ++i) {
        if (hand[i].is_face_or_ace() && hand[i].is_trump(upcard_suit)) {
          ++trump_face_aces;
        }
      }
      if (trump_face_aces >= 2) {
        order_up_suit = upcard_suit;
        return true;
      }
      return false;
    }

    const Suit next_suit = Suit_next(upcard_suit);
    int trump_face_aces = 0;
    for (size_t i = 0; i < hand.size(); ++i) {
      if (hand[i].is_face_or_ace() && hand[i].is_trump(next_suit)) {
        ++trump_face_aces;
      }
    }
    if (trump_face_aces >= 1 || is_dealer) {
      order_up_suit = next_suit;
      return true;
    }
    return false;
  }

  void add_and_discard(const Card &upcard) override {
    hand.push_back(upcard);
    size_t lowest_index = 0;
    for (size_t i = 1; i < hand.size(); ++i) {
      if (Card_less(hand[i], hand[lowest_index], upcard.get_suit())) {
        lowest_index = i;
      }
    }
    hand.erase(hand.begin() + lowest_index);
  }

  Card lead_card(Suit trump) override {
    bool has_non_trump = false;
    size_t best_index = 0;
    for (size_t i = 0; i < hand.size(); ++i) {
      if (!hand[i].is_trump(trump)) {
        if (!has_non_trump || Card_less(hand[best_index], hand[i], trump)) {
          best_index = i;
          has_non_trump = true;
        }
      }
    }
    if (!has_non_trump) {
      for (size_t i = 1; i < hand.size(); ++i) {
        if (Card_less(hand[best_index], hand[i], trump)) {
          best_index = i;
        }
      }
    }
    Card chosen = hand[best_index];
    hand.erase(hand.begin() + best_index);
    return chosen;
  }

  Card play_card(const Card &led_card, Suit trump) override {
    const Suit led_suit = led_card.get_suit(trump);
    bool can_follow = false;
    size_t best_index = 0;
    for (size_t i = 0; i < hand.size(); ++i) {
      if (hand[i].get_suit(trump) == led_suit) {
        if (!can_follow ||
            Card_less(hand[best_index], hand[i], led_card, trump)) {
          best_index = i;
          can_follow = true;
        }
      }
    }
    if (!can_follow) {
      for (size_t i = 1; i < hand.size(); ++i) {
        if (Card_less(hand[i], hand[best_index], led_card, trump)) {
          best_index = i;
        }
      }
    }
    Card chosen = hand[best_index];
    hand.erase(hand.begin() + best_index);
    return chosen;
  }

private:
  string name;
  vector<Card> hand;
};

class HumanPlayer : public Player {
public:
  explicit HumanPlayer(const string &name_in) : name(name_in) {}
  const string &get_name() const override { return name; }
  void add_card(const Card &c) override {
    hand.push_back(c);
    sort(hand.begin(), hand.end());
  }

  bool make_trump(const Card &upcard, bool is_dealer, int round,
                  Suit &order_up_suit) const override {
    (void)upcard; (void)is_dealer; (void)round;
    print_hand();
    cout << "Human player " << name
         << ", please enter a suit, or \"pass\":" << endl;
    string decision;
    cin >> decision;
    if (decision == "pass") return false;
    order_up_suit = string_to_suit(decision);
    return true;
  }

  void add_and_discard(const Card &upcard) override {
    print_hand();
    cout << "Discard upcard: [-1]" << endl;
    cout << "Human player " << name
         << ", please select a card to discard:" << endl;
    int discard_choice = 0;
    cin >> discard_choice;
    if (discard_choice != -1) {
      hand.erase(hand.begin() + discard_choice);
      hand.push_back(upcard);
      sort(hand.begin(), hand.end());
    }
  }

  Card lead_card(Suit trump) override {
    (void)trump;
    print_hand();
    cout << "Human player " << name << ", please select a card:" << endl;
    int choice = 0;
    cin >> choice;
    Card chosen = hand[choice];
    hand.erase(hand.begin() + choice);
    return chosen;
  }

  Card play_card(const Card &led_card, Suit trump) override {
    (void)led_card; (void)trump;
    print_hand();
    cout << "Human player " << name << ", please select a card:" << endl;
    int choice = 0;
    cin >> choice;
    Card chosen = hand[choice];
    hand.erase(hand.begin() + choice);
    return chosen;
  }

private:
  string name;
  vector<Card> hand;
  void print_hand() const {
    for (size_t i = 0; i < hand.size(); ++i) {
      cout << "Human player " << name << "'s hand: [" << i << "] "
           << hand[i] << endl;
    }
  }
};

Player *Player_factory(const string &name, const string &strategy) {
  if (strategy == "Simple") return new SimplePlayer(name);
  if (strategy == "Human") return new HumanPlayer(name);
  assert(false);
  return nullptr;
}

ostream &operator<<(ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}