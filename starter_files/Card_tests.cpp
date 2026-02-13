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
    ASSERT_TRUE(jack_of_diamonds.is_left_bower(trump));
    ASSERT_TRUE(jack_of_diamonds.is_trump(trump));
    ASSERT_EQUAL(jack_of_diamonds.get_suit(trump), HEARTS);

    // if it's not a trump, it's a normal diamond
    ASSERT_TRUE(jack_of_diamonds.get_suit() == DIAMONDS);
}

TEST(test_card_less_trump) {
    Card ace_spades(ACE, SPADES);
    Card nine_hearts(NINE, HEARTS); // Trump
    Card jack_hearts(JACK, HEARTS); // Right Bower
    Card jack_diamonds(JACK, DIAMONDS); // Left Bower
    Suit trump = HEARTS;

    // Right Bower > Left Bower (So a < b is true)
    ASSERT_TRUE(Card_less(jack_diamonds, jack_hearts, trump));

    // Left Bower > Ace of Trump
    Card ace_hearts(ACE, HEARTS);
    ASSERT_TRUE(Card_less(ace_hearts, jack_diamonds, trump));

    // 9 of Trump > Ace of Non-Trump
    ASSERT_TRUE(Card_less(ace_spades, nine_hearts, trump));

}

TEST(test_card_less_led_suit) {
    Card nine_diamonds(NINE, DIAMONDS);
    Card ace_spades(ACE, SPADES);
    Card led_card(FIVE, DIAMONDS); // This card defines the led suit
    Suit trump = HEARTS;

    // Pass the led_card, not the suit
    ASSERT_TRUE(Card_less(ace_spades, nine_diamonds, led_card, trump));

}

TEST(test_left_bower_vs_natural_suit) {
    Card jack_diamonds(JACK, DIAMONDS); // Left Bower
    Card ace_diamonds(ACE, DIAMONDS);   // High natural suit
    Suit trump = HEARTS;

    // Left Bower should be stronger than the ace of its own natural suit
    ASSERT_TRUE(Card_less(ace_diamonds, jack_diamonds, trump));
}

TEST(test_operators) {
    Card c1(KING, SPADES);
    Card c2(KING, SPADES);
    Card c3(ACE, SPADES);

    ASSERT_TRUE(c1 == c2);
    ASSERT_TRUE(c1 < c3);
    ASSERT_FALSE(c1 > c3);
}


TEST(test_left_bower_black_suits) {
    Card jack_clubs(JACK, CLUBS);
    Suit trump = SPADES;

    // Verify logic works for black suits as well
    ASSERT_TRUE(jack_clubs.is_left_bower(trump));
    ASSERT_EQUAL(jack_clubs.get_suit(trump), SPADES);
}

TEST(test_card_less_no_trump_no_led) {
    Card ace_clubs(ACE, CLUBS);
    Card nine_spades(NINE, SPADES);
    Card led_card(SEVEN, DIAMONDS); // Lead was Diamonds
    Suit trump = HEARTS;

    // Neither is trump, neither is diamonds. 
    // Ace of Clubs should beat Nine of Spades.
    ASSERT_TRUE(Card_less(nine_spades, ace_clubs, led_card, trump));
}

TEST(test_card_less_self_comparison) {
    Card ace_hearts(ACE, HEARTS);
    Suit trump = HEARTS;
    
    // A card should not be less than itself
    ASSERT_FALSE(Card_less(ace_hearts, ace_hearts, trump));
}

TEST(test_left_bower_led_suit_behavior) {
    Card jack_diamonds(JACK, DIAMONDS); // Left Bower
    Card ace_diamonds(ACE, DIAMONDS);   // High Diamond
    Card led_card(NINE, DIAMONDS);      // Diamonds were led
    Suit trump = HEARTS;

    // The Jack of Diamonds is a TRUMP, not a DIAMOND.
    // In a comparison where Diamonds are led, the Jack of Diamonds 
    // should still beat the Ace of Diamonds because Trump > Led Suit.
    ASSERT_TRUE(Card_less(ace_diamonds, jack_diamonds, led_card, trump));
}

TEST(test_card_less_natural_rank) {
    Card queen_spades(QUEEN, SPADES);
    Card seven_clubs(SEVEN, CLUBS);
    Card led_card(NINE, DIAMONDS); 
    Suit trump = HEARTS;

    // Neither is trump or led suit. Queen should beat Seven.
    ASSERT_TRUE(Card_less(seven_clubs, queen_spades, led_card, trump));
}

// Add more test cases here
TEST_MAIN()
