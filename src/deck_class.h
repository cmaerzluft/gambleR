#ifndef Deck_H
#define Deck_H

// CM NOTE: This will eventually change to use #include <RcppArmadillo.h>
#include <vector>

class Deck {
public:
  // Constructor
  Deck(int n_decks = 1, bool shuffle = true);

  // Public Members
  int n_decks;
  std::vector<int> base_deck;
  bool needs_shuffle = false;

  // Getters
  int get_ndecks();
  int get_ncards();
  std::vector<int> get_basedeck();

  // CM NOTE: To remove when done testing
  std::vector<int> get_currentdeck();

  // Methods
  bool needs_shuffling();
  std::vector<int> shuffle_deck(std::vector<int> cards);

private:
  // Private Members
  std::vector<int> deck;
};

#endif