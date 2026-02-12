#include <cassert>
#include <iostream>
#include <array>
#include <string>
#include "Card.hpp"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false);
  return {};
}

std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
  return is;
}

/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false);
  return {};
}

std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}

/////////////// Write your implementation for Card below ///////////////
// default constructor
Card::Card() : rank(TWO), suit(SPADES) {
}

// Default constructor
Card::Card() : rank(TWO), suit(SPADES) {}

// Parameterized constructor
Card::Card(Rank rank_in, Suit suit_in) : rank(rank_in), suit(suit_in) {}

Rank Card::get_rank() const {
  return rank;
}

Suit Card::get_suit() const {
  return suit;
}

// If the card is the Left Bower, its suit is effectively the trump suit
Suit Card::get_suit(Suit trump) const {
  if (is_left_bower(trump)) {
    return trump;
  }
  return suit;
}

bool Card::is_face_or_ace() const {
  return rank >= JACK;
}

bool Card::is_right_bower(Suit trump) const {
  return (rank == JACK && suit == trump);
}

bool Card::is_left_bower(Suit trump) const {
  return (rank == JACK && suit == Suit_next(trump));
}

bool Card::is_trump(Suit trump) const {
  return (suit == trump || is_left_bower(trump));
}

// Global Helper: Returns the suit of the same color
Suit Suit_next(Suit suit) {
  if (suit == SPADES)   return CLUBS;
  if (suit == CLUBS)    return SPADES;
  if (suit == HEARTS)   return DIAMONDS;
  if (suit == DIAMONDS) return HEARTS;
  return SPADES; 
}

// --- Overloaded Operators (Non-Trump Comparisons) ---

std::ostream & operator<<(std::ostream &os, const Card &card) {
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

std::istream & operator>>(std::istream &is, Card &card) {
  string trash; 
  if (is >> card.rank >> trash >> card.suit) {
    // Successfully read rank, "of", and suit
  }
  return is;
}

bool operator<(const Card &lhs, const Card &rhs) {
  if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() < rhs.get_rank();
  }
  return lhs.get_suit() < rhs.get_suit();
}

bool operator<=(const Card &lhs, const Card &rhs) {
  return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const Card &lhs, const Card &rhs) {
  return !(lhs <= rhs);
}

bool operator>=(const Card &lhs, const Card &rhs) {
  return !(lhs < rhs);
}

bool operator==(const Card &lhs, const Card &rhs) {
  return (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit());
}

bool operator!=(const Card &lhs, const Card &rhs) {
  return !(lhs == rhs);
}

// --- Card_less Logic (Trump-Aware) ---

bool Card_less(const Card &a, const Card &b, Suit trump) {
    // Right Bower is the highest
    if (b.is_right_bower(trump)) return true;
    if (a.is_right_bower(trump)) return false;

    // Left Bower is the second highest
    if (b.is_left_bower(trump)) return true;
    if (a.is_left_bower(trump)) return false;

    // Trump always beats non-trump
    if (b.is_trump(trump) && !a.is_trump(trump)) return true;
    if (a.is_trump(trump) && !b.is_trump(trump)) return false;

    // If both are trump or both are non-trump, use standard comparison
    return a < b;
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
    // If at least one is trump, use the trump-aware Card_less
    if (a.is_trump(trump) || b.is_trump(trump)) {
        return Card_less(a, b, trump);
    }

    // Neither is trump. Check the led suit.
    Suit led_suit = led_card.get_suit(trump);

    if (b.get_suit(trump) == led_suit && a.get_suit(trump) != led_suit) {
        return true; 
    }
    if (a.get_suit(trump) == led_suit && b.get_suit(trump) != led_suit) {
        return false;
    }

    // Otherwise, compare normally
    return a < b;
}