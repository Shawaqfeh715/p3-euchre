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

// Stream input constructor
Pack::Pack(std::istream& pack_input) {
    next = 0;
    std::string rank_str, junk, suit_str;
    for (int i = 0; i < PACK_SIZE; ++i) {
        if(pack_input >> rank_str >> junk >> suit_str) {
            cards[i] = Card(string_to_rank(rank_str), string_to_suit(suit_str));
        }
    }
}

Card Pack::deal_one() {
    assert(!empty());
    Card dealt = cards[next];
    next ++;
    return dealt;

}

bool Pack::empty() const { 
    return next>=PACK_SIZE;
}

void Pack::reset(){
    next=0;
}

void Pack::shuffle() {
    for (int count = 0; count < 7; ++count) {
        std::array<Card, PACK_SIZE> temp;
        for (int i = 0; i < PACK_SIZE; ++i) {
            if (i % 2 == 0) {
                // Even indices get the bottom half (starting at index 12)
                temp[i] = cards[PACK_SIZE / 2 + i / 2];
            } else {
                // Odd indices get the top half (starting at index 0)
                temp[i] = cards[i / 2];
            }
        }
        // Move this OUTSIDE the i-loop so the shuffle finishes before updating
        cards = temp; 
    }
    reset(); // Sets next = 0
}

