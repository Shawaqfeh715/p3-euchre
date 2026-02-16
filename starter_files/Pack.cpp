#include "Pack.hpp"
#include <string>
#include <cassert>

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on. 
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

// REQUIRES: pack_input contains a representation of a Pack
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
Pack::Pack(std::istream& pack_input) {
    next = 0;
    std::string rank_str, junk, suit_str;
    for (int i = 0; i < PACK_SIZE; ++i) {
        if (pack_input >> rank_str >> junk >> suit_str) {
            cards[i] = Card(string_to_rank(rank_str), string_to_suit(suit_str));
        }
    }
} // <--- Added missing brace here

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one() {
    assert(!empty());
    Card dealt = cards[next];
    next++;
    return dealt;
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
    return next >= PACK_SIZE;
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset() {
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times.
void Pack::shuffle() {
    for (int count = 0; count < 7; ++count) {
        std::array<Card, PACK_SIZE> temp;
        for (int i = 0; i < PACK_SIZE; ++i) {
            if (i % 2 == 0) {
                // Even indices get the bottom half (starting at index 12)
                temp[i] = cards[PACK_SIZE / 2 + (i / 2)];
            } else {
                // Odd indices get the top half (starting at index 0)
                temp[i] = cards[i / 2];
            }
        }
        cards = temp; 
    }
    reset(); 
}