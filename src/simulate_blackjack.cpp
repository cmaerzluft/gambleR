#include <numeric>
#include <RcppArmadilloExtensions/sample.h>
#include <Rcpp.h>
using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//
// sim_hand_blackjack <- function(game, cards, hand_id, player_ids, n_players_dealer) {
// # First deal
//   deal <- sample(length(cards), n_players_dealer*2)
//   game$card_value <- cards[deal]
//   remaining_cards <- cards[-deal]
//
// # Score (to help determine hit strategy)
//   scores <- score_blackjack(game = game)
//     to_hit <- scores$player_score < 17 | (scores$player_score == 17 & scores$soft_value)
//
// # Stop game if deal has 21 at start
//     if (scores$player_score[length(scores$player_score)] == 21) {
//       return(list(game = game, cards = remaining_cards))
//
//     } else {
// # iterate over players in the hand, hitting based on hit strategy
//       while (any(to_hit)) {
// # Error check
//         if (length(remaining_cards) == 0) { stop(
//           "Not enough cards. Change 4 to a larger number in length(res$cards) < n_players_dealer*4 in simulate_blackjack.R"
//         ) }
//
// # Get player's hand that needs to be hit on
//         player_up <- as.integer(names(which.max(to_hit)))
//           player_hand <- game$card_value[game$player_id == player_up] %% 100
//
// # Deal a card from the deck
//         deal_card_idx <- sample(length(remaining_cards), 1)
//           game[nrow(game) + 1, ] <- c(hand_id, player_up, length(player_hand) + 1L, remaining_cards[deal_card_idx])
//           remaining_cards <- remaining_cards[-deal_card_idx]
//
// # Score and decide whether to hit again
//         scores <- score_blackjack(game = game$card_value[game$player_id == player_up])
//           to_hit[names(to_hit) == player_up] <- scores$player_score < 17 | (scores$player_score == 17 & scores$soft_value)
//       }
//     }
//
//     return(list(game = game, cards = remaining_cards))
// }

// [[Rcpp::export]]
std::vector<int> simulate_blackjack(double n_decks, double n_players, double n_hands, std::vector<int> card_deck) {
  // Rcpp::DataFrame
  // Initialize player IDs
  int n_players_dealer = n_players + 1;
  IntegerVector player_ids(n_players_dealer);
  std::iota(std::begin(player_ids), std::end(player_ids), 1);
  player_ids[n_players] = 101;

  // Initialize all simulated games information
  std::vector<int> game_hand_id;
  std::vector<int> game_player_id;
  std::vector<int> game_card_number;
  std::vector<int> game_card_value;
  std::vector<int> game_shuffle_id;
  game_hand_id.reserve(n_hands*n_players_dealer*2);
  game_player_id.reserve(n_hands*n_players_dealer*2);
  game_card_number.reserve(n_hands*n_players_dealer*2);
  game_card_value.reserve(n_hands*n_players_dealer*2);
  game_shuffle_id.reserve(n_hands*n_players_dealer*2);

  // Shuffle and burn one card
  std::vector<int> shuffled_deck = RcppArmadillo::sample(card_deck, card_deck.size(), false);
  shuffled_deck.erase(shuffled_deck.begin());

  // // Simulate hands
  bool new_deck = true;
  int current_shuffle = 1;
  for (int i1 = 0; i1 < n_hands; ++i1) {
  //   // Check if we need to shuffle a new deck
  //   if (shuffled_deck.size() < n_players_dealer*(4 + 4/n_players_dealer)) {
    int x = shuffled_deck.size();
    int y = n_players_dealer*(15 + 4/n_players_dealer);
    std::cout << y << '\n';
    if (x < y) {
      std::cout << "HERE" << '\n';
      shuffled_deck.clear();
      std::vector<int> shuffled_deck = RcppArmadillo::sample(card_deck, card_deck.size(), false);
      shuffled_deck.erase(shuffled_deck.begin());
      std::cout << shuffled_deck.size() << '\n';
    }

    // First iteration of a hand
    for (int i2 = 0; i2 < 2; ++i2) {
      for (int i3 = 0; i3 < n_players_dealer; ++i3) {
        game_hand_id.push_back(i1 + 1);
        game_card_number.push_back(i2 + 1);
        game_player_id.push_back(player_ids[i3]);
        game_card_value.push_back(shuffled_deck[0]);
        game_shuffle_id.push_back(current_shuffle);
        shuffled_deck.erase(shuffled_deck.begin());
        std::cout << shuffled_deck.size() << '\n';
      }
    }
  }

  // IntegerVector ret = RcppArmadillo::sample(player_ids, n_players_dealer, progress);
  // return Rcpp::DataFrame::create(
  //   Rcpp::Named("shuffle_id") = game_shuffle_id,
  //   Rcpp::Named("hand_id") = game_hand_id,
  //   Rcpp::Named("player_id") = game_player_id,
  //   Rcpp::Named("card_number") = game_card_number,
  //   Rcpp::Named("card_value") = game_card_value
  // );
  return shuffled_deck;
}


// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically
// run after the compilation.
//

/*** R
simulate_blackjack(1, 1, 20, gambleR::card_deck$card_id)
*/
