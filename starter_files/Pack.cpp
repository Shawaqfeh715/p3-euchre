#include "Pack.hpp"
#include <string>
#include <cassert>

// helper functions
// These help the stream constructor convert strings to Enums
static Rank string_to_rank(std::string str) {
    if (str == "Nine") return NINE;
    if (str == "Ten") return TEN;
    if (str == "Jack") return JACK;
    if (str == "Queen") return QUEEN;
    if (str == "King") return KING;
    if (str == "Ace") return ACE;
    assert(false); // Should never happen if input is valid
}


static Suit string_to_suit(std::string str) {
    if (str == "Spades") return SPADES;
    if (str == "Hearts") return HEARTS;
    if (str == "Clubs") return CLUBS;
    if (str == "Diamonds") return DIAMONDS;
    assert(false); // Should never happen if input is valid
}

Pack::Pack() {
    next = 0;
    int i = 0;
    for (int s = SPADES; s <= DIAMONDS; ++s) {
        for (int r = NINE; r <= ACE; ++r) {
            cards[i] = Card(static_cast<Rank>(r), static_cast<Suit>(s));
            ++i;
        }
    }
}