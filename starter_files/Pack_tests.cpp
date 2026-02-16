#include "Pack.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
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


TEST(test_pack_reset) {
    Pack pack;
    Card first_card = pack.deal_one();
    
    pack.reset();
    Card reset_card = pack.deal_one();
    
    ASSERT_EQUAL(first_card, reset_card);
}


TEST(test_pack_stream_reader) {
    // A tiny pack of cards in a string
    std::string data = "Nine of Spades\nTen of Spades\nJack of Spades\n"
                       "Queen of Spades\nKing of Spades\nAce of Spades\n"
                       
                       "Nine of Hearts\nTen of Hearts\nJack of Hearts\n"
                       "Queen of Hearts\nKing of Hearts\nAce of Hearts\n"
                       "Nine of Clubs\nTen of Clubs\nJack of Clubs\n"
                       "Queen of Clubs\nKing of Clubs\nAce of Clubs\n"
                       "Nine of Diamonds\nTen of Diamonds\nJack of Diamonds\n"
                       "Queen of Diamonds\nKing of Diamonds\nAce of Diamonds\n";
    
    std::istringstream iss(data);
    Pack pack(iss);
    
    Card first = pack.deal_one();
    ASSERT_EQUAL(first, Card(NINE, SPADES));
}


TEST_MAIN()
