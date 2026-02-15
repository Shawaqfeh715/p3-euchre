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
    Card first_before = pack.deal_one(); // 9 of Spades
    pack.reset();

    pack.shuffle();
    Card first_after = pack.deal_one();

    // In a 24-card In-Shuffle, the card at index 12 moves to index 0.
    // Index 12 in a new deck is the 9 of Hearts.
    ASSERT_NOT_EQUAL(first_before, first_after);
    ASSERT_EQUAL(first_after, Card(NINE, HEARTS));

}


TEST_MAIN()
