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

