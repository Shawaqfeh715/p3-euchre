#include "Pack.hpp"
#include <string>
#include <cassert>

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

Pack::Pack(std::istream& pack_input) {
    next = 0;
    std::string rank_str, junk, suit_str;
    for (int i = 0; i < PACK_SIZE; ++i) {
        if (pack_input >> rank_str >> junk >> suit_str) {
            cards[i] = Card(string_to_rank(rank_str), string_to_suit(suit_str));
        }
    }
}

Card Pack::deal_one() {
    assert(!empty());
    Card dealt = cards[next];
    next++;
    return dealt;
}

bool Pack::empty() const {
    return next >= PACK_SIZE;
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle() {
    for (int count = 0; count < 7; ++count) {
        std::array<Card, PACK_SIZE> temp;
        for (int i = 0; i < PACK_SIZE; ++i) {
            if (i % 2 == 0) {
                temp[i] = cards[PACK_SIZE / 2 + (i / 2)];
            } else {
                temp[i] = cards[i / 2];
            }
        }
        cards = temp; 
    }
    reset(); 
}