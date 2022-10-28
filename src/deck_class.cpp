// CM NOTE: This will eventually change to use #include <RcppArmadilloExtensions/sample.h> instead of "deck_utils.h"
#include "deck_utils.h"
#include "deck_class.h"
using namespace Rcpp;

// Constructor - prepare n_decks of cards
Deck::Deck(int n_decks, bool shuffle) {
  // Define base deck card IDs:
  //  100s digit represents suit (1, 2, 3, 4 ~ Hearts, Clubs, Diamonds, Spades)
  //  10s and 1s digits represents value (14, 2-10, 11, 12, 13 ~ Ace, 2-10, Jack, Queen, King)
  std::vector<int> base_deck, card_ids = {
    114, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
    214, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
    314, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313,
    414, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413
  };

  // Repeat base deck n_decks times to get the full shoe
  while(n_decks--) { base_deck.insert(base_deck.end(), card_ids.begin(), card_ids.end()); }

  // Shuffle the deck if desired
  std::vector<int> deck;
  if (shuffle) {
    deck = Deck::shuffle_deck(base_deck);
  } else {
    deck = base_deck;
  }

  // Assign members
  this -> n_decks = n_decks;
  this -> base_deck = base_deck;
  this -> deck = deck;
}

////// Getters //////
int Deck::get_ndecks() { return n_decks; }
int Deck::get_ncards() { return deck.size(); }
std::vector<int> Deck::get_basedeck() { return base_deck; }

// CM NOTE: To remove when done testing
std::vector<int> Deck::get_currentdeck() { return deck; }

////// Methods //////
// Check Shuffle status
bool Deck::needs_shuffling() {
  // to do: write code that checks the size of the deck vs a total number of players
  //  (and eventually given a specific game) to see if it should be reshuffled

  // Should set needs_shuffle for class
  return needs_shuffle;
}

// Shuffle deck of cards
std::vector<int> Deck::shuffle_deck(std::vector<int> cards) {

  // Shuffle the cards
  // CM NOTE: This will eventually change to use RcppArmadillo::sample(fresh_deck, fresh_deck.size(), false)
  std::vector<int> new_shuffle = sample(cards);
  new_shuffle.erase(new_shuffle.begin());

  // Change shuffle status for deck
  this -> needs_shuffle = false;
  return (new_shuffle);
}