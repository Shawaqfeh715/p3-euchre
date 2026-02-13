#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}
TEST(test_card_default_ctor){
    Card c;
    ASSERT_EQUAL(TWO,c.get_rank());
    ASSERT_EQUAL(SPADES,c.get_suit());
}
TEST(test_is_face_or_ace_true_cases){
    Card  jack(JACK,SPADES);
    Card  king(KING,CLUBS);
    Card queen(QUEEN,HEARTS);
    Card ace(ACE,DIAMONDS);

    ASSERT_TRUE(jack.is_face_or_ace());
    ASSERT_TRUE(queen.is_face_or_ace());
    ASSERT_TRUE(king.is_face_or_ace());
    ASSERT_TRUE(ace.is_face_or_ace());
}

TEST(test_is_face_or_ace_false_cases){
    Card nine(NINE,SPADES);
    Card ten(TEN,HEARTS);

    ASSERT_FALSE(nine.is_face_or_ace());
    ASSERT_FALSE(ten.is_face_or_ace());
}

TEST(test_right_bower){
    Card jack_hearts(JACK,HEARTS);

    ASSERT_TRUE(jack_hearts.is_right_bower(HEARTS));

    ASSERT_FALSE(jack_hearts.is_right_bower(DIAMONDS));
    ASSERT_FALSE(jack_hearts.is_right_bower(SPADES));
    ASSERT_FALSE(jack_hearts.is_right_bower(CLUBS));
}

TEST(test_left_bower){
    Card jack_diamonds(JACK,DIAMONDS);

    ASSERT_TRUE(jack_diamonds.is_left_bower(HEARTS));

    ASSERT_FALSE(jack_diamonds.is_left_bower(DIAMONDS));
}
TEST_MAIN()
