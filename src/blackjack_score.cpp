#include <numeric>
#include <Rcpp.h>
using namespace Rcpp;

List score_blackjack(std::vector<int> player_hand) {
  // Initializations
  int n_cards = player_hand.size();
  int player_score = 0;
  bool has_ace = false;
  bool soft_value = false;

  // Add scores and track aces
  for (int i1 = 0; i1 < n_cards; i1++) {
    if (player_hand[i1] == 1) { has_ace = true; }
    player_score += player_hand[i1];
  }

  // Count soft-aces as 11 (1 + 10)
  soft_value = has_ace & ((player_score + 10) <= 21);
  if (soft_value) { player_score += 10; }

  return List::create(
    _["player_score"] = player_score,
    _["soft_value"] = soft_value
  );
}

// [[Rcpp::export]]
List score_blackjack_by_group(std::vector<int> player_hands, std::vector<int> player_hand_ids, std::vector<int> player_ids) {
  // CM NOTE: Can probably come up with a better name for export version.
  // Initializations
  int n_players = player_ids.size();
  std::vector<int> player_scores;
  std::vector<int> soft_values;

  // Get each players score
  for (int i1 = 0; i1 < n_players; i1++) {
    // Get current player's hand
    std::vector<int> player_i1_hand;
    for (int i2 = 0; i2 < player_hands.size(); i2++) {
      // CM NOTE: Can we remove cards from player_hands so each succesive iteration is faster?
      if (player_hand_ids[i2] == player_ids[i1]) {
        player_i1_hand.push_back(player_hands[i2]);
      }
    }
    // Score the player's hand
    List group_i1_score = score_blackjack(player_i1_hand);
    player_scores.push_back(group_i1_score["player_score"]);
    soft_values.push_back(group_i1_score["soft_value"]);
  }

  return List::create(
    _["player_score"] = player_scores,
    _["soft_value"] = soft_values
  );
}