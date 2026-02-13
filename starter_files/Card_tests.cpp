#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(identity_of_left_bower) {
    Card jack_of_diamonds(JACK, DIAMONDS);
    Suit trump = HEARTS;

    // edge case: suit should be reported as hearts even though it's a DIAMOND
    ASSERT_TRUE(jack_diamonds.is_left_bower(trump));
    ASSERT_TRUE(jack_diamonds.is_trump(trump));
    ASSERT_EQUAL(jack_diamonds.get_suit(trump), HEARTS);

    // if it's not a trump, it's a normal diamond
    ASSERT_TRUE(jack_of_diamonds.get_suit() == DIAMONDS);
}
// Add more test cases here
TEST_MAIN()
