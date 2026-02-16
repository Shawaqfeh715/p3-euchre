#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

TEST(test_pack_shuffle) {
    Pack pack;
    pack.shuffle(); // This runs the internal 7-shuffle loop
    
    Card first_card = pack.deal_one();
    
    // After exactly 7 In-Shuffles of a standard 24-card deck, 
    // the card at the top should be the King of Clubs.
    ASSERT_EQUAL(first_card, Card(KING, CLUBS));
}

TEST(test_pack_is_empty) {
    Pack pack;
    for (int i = 0; i < 24; ++i) {
        ASSERT_FALSE(pack.empty());
        pack.deal_one();
    }
    ASSERT_TRUE(pack.empty());
}

TEST_MAIN()
