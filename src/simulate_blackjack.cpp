#include <numeric>
#include <RcppArmadilloExtensions/sample.h>
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
int id_to_value_blackjack(int x) {
  x = x % 100;
  if (x == 11 | x == 12 | x == 13) { x = 10; }
  if (x == 14) { x = 1; }

  return x;
}

// [[Rcpp::export]]
List score_indvidual_blackjack(std::vector<int> x) {
  // Initializations
  int n_scores = x.size();
  int player_score = 0;
  bool has_ace = false;
  bool soft_value = false;

  // Add scores and track aces
  for (int i1 = 0; i1 < n_scores; ++i1) {
    if (x[i1] == 1) { has_ace = true; }
    player_score += x[i1];
  }

  // Count soft-aces as 11 (1 + 10)
  soft_value = has_ace & ((player_score + 10) <= 21);
  if (soft_value) { player_score += 10; }

  return List::create(
    _["player_score"] = player_score,
    _["soft_value"] = soft_value
  )
}

List score_groups_blackjack(std::vector<int> x, std::vector<int> y) {
  // Initializations
  // CM NOTE: Need to verify if this is what we want
  // Alt do we want output to be same length as x???
  std::vector<int> group_ids = sort_unique(noNA(y));
  int n_groups = group_ids.size();
  std::vector<int> player_score;
  std::vector<int> soft_value;

  for (int i1 = 0; i1 < n_groups; ++i1) {
    std::vector<int> group_i1_hand;
    for (int i2 = 0; i2 < x.size(); ++i2) {
      if (y[i2] == group_ids[i1]) {
        group_i1_hand.push_back(x[i2]);
      }
    }
    List group_i1_score = score_indvidual_blackjack(group_i1_hand);

    player_score.push_back(group_i1_score["player_score"]);
    soft_value.push_back(group_i1_score["soft_value"]);
  }

  return List::create(
    _["player_score"] = player_score,
    _["soft_value"] = soft_value
  )
}

// [[Rcpp::export]]
List score_blackjackC(IntegerVector x, IntegerVector y = IntegerVector::create(0)) {
  // CM NOTE: For some reason calling this function directly in R as part of a group_by is very memory inefficient.
  //  need to find out why
  // Initializations
  int n_scores = x.size();
  int n_groups = y.size();
  IntegerVector group_ids;
  if (n_groups > 1) {
    group_ids = sort_unique(noNA(y));
    n_groups = group_ids.size();
  }
  // IntegerVector value(n_scores);
  LogicalVector has_ace(n_groups);
  LogicalVector soft_value(n_groups);
  IntegerVector player_score(n_groups);

  // Add scores and track aces
  for (int i1 = 0; i1 < n_scores; i1++) {
    for (int i2 = 0; i2 < n_groups; i2++) {
      if (x[i1] == 1) {
        has_ace[i2] = true;
      }
      if (n_groups > 1) {
        if (y[i1] == group_ids[i2]) {
          player_score[i2] += x[i1];
        }
      } else {
        player_score[i2] += x[i1];
      }
    }
  }

  // Count soft-aces as 11 (1 + 10)
  for (int i1 = 0; i1 < n_groups; i1++) {
    soft_value[i1] = has_ace[i1] & ((player_score[i1] + 10) <= 21);
    if (soft_value[i1]) {
      player_score[i1] += 10;
    }
  }

  return List::create(
    _["player_score"] = player_score,
    _["soft_value"] = soft_value
  );
}

// [[Rcpp::export]]
Rcpp::DataFrame simulate_blackjackC(double n_players, double n_hands, std::vector<int> card_deck) {
  // Initialize player IDs
  int n_players_dealer = n_players + 1;
  IntegerVector player_ids(n_players_dealer);
  std::iota(std::begin(player_ids), std::end(player_ids), 1);
  player_ids[n_players] = 101;

  // Initialize all simulated games information
  std::vector<int> game_hand_id;
  std::vector<int> game_player_id;
  std::vector<int> game_card_number;
  std::vector<int> game_card_id;
  std::vector<int> game_card_value;
  std::vector<int> game_shuffle_id;
  game_hand_id.reserve(n_hands*n_players_dealer*2);
  game_player_id.reserve(n_hands*n_players_dealer*2);
  game_card_number.reserve(n_hands*n_players_dealer*2);
  game_card_id.reserve(n_hands*n_players_dealer*2);
  game_card_value.reserve(n_hands*n_players_dealer*2);
  game_shuffle_id.reserve(n_hands*n_players_dealer*2);

  // Shuffle and burn one card
  std::vector<int> shuffled_deck = RcppArmadillo::sample(card_deck, card_deck.size(), false);
  shuffled_deck.erase(shuffled_deck.begin());

  // Simulate hands
  int current_shuffle = 1;
  for (int i1 = 0; i1 < n_hands; ++i1) {
    // Individual hand information
    // CM NOTE: Doing this so we can pass this information as needed later on, but is there a fast way to use subsets
    //  of a vector because hand_ objects are really just subsets of game_ objects which is what we return ultimately
    std::vector<int> hand_player_id;
    std::vector<int> hand_card_number;
    std::vector<int> hand_card_id;
    std::vector<int> hand_card_value;
    hand_player_id.reserve(n_players_dealer*2);
    hand_card_number.reserve(n_players_dealer*2);
    hand_card_id.reserve(n_players_dealer*2);
    hand_card_value.reserve(n_players_dealer*2);

    // Reshuffle the deck if we need to
    if (shuffled_deck.size() < n_players_dealer*(4 + 4/n_players_dealer)) {
      // CM NOTE: Can we add this routine as a method to std::vector? so we can do something like:
      //  shuffled_deck.reshuffle(card_deck);
      //  which would clear the shuffled_deck, shuffle card_deck, then insert and return it to shuffled_deck. It would
      //  really just be prettier I think.
      shuffled_deck.clear();
      std::vector<int> new_shuffle = RcppArmadillo::sample(card_deck, card_deck.size(), false);
      shuffled_deck.insert(shuffled_deck.begin(), new_shuffle.begin(), new_shuffle.end());
      shuffled_deck.erase(shuffled_deck.begin());
      current_shuffle += 1;
    }

    // Deal the cards
    for (int i2 = 0; i2 < 2; ++i2) {
      for (int i3 = 0; i3 < n_players_dealer; ++i3) {
        int card_i_i3 = shuffled_deck[0];
        int card_v_i3 = id_to_value_blackjack(card_i_i3);

        // Deal cards tracking in the game results
        game_shuffle_id.push_back(current_shuffle);
        game_hand_id.push_back(i1 + 1);
        game_player_id.push_back(player_ids[i3]);
        game_card_number.push_back(i2 + 1);
        game_card_id.push_back(card_i_i3);
        game_card_value.push_back(card_v_i3);
        // and in the current hand
        hand_player_id.push_back(player_ids[i3]);
        hand_card_id.push_back(card_i_i3);
        hand_card_value.push_back(card_v_i3);
        // Remove dealt card from deck
        shuffled_deck.erase(shuffled_deck.begin());
      }
    }

    // Score players and determine who wants to hit
    List scores = score_blackjackC(wrap(hand_card_value), wrap(hand_player_id));
    IntegerVector score_value = scores["player_score"];
    IntegerVector soft_ace = scores["soft_value"];

    // Stop this hand if the dealer has 21
    if (score_value[score_value.size() - 1] >= 21) {
      continue;
    }

    // Determine who wants to hit
    // CM NOTE: Make this a function that takes a condition in the form of a function?
    //  i.e. hit_strategy(scores, soft, f) where f = function(x) x < 17 | (x == 17 & soft)
    //  this should also be something that can be specified for each player individually
    std::vector<int> idxs_to_hit;
    bool any_to_hit;
    for (int i4 = 0; i4 < score_value.size(); ++i4) {
      if ((score_value[i4] < 17) | (score_value[i4] == 17 & soft_ace[i4] == true)) {
        idxs_to_hit.push_back(i4);
        if (any_to_hit == false) {
          any_to_hit = true;
        }
      }
    }

    // Continue to hit everyone in order until they are satisfied with their cards
    int safety_stop = 0;
    while (any_to_hit == true) {
      safety_stop += 1;
      if (safety_stop > 1000) {
        any_to_hit = false;
      }

      // Get first person information
      int idx_to_hit = idxs_to_hit[0];
      int player_up = player_ids[idx_to_hit];
      std::vector<int> player_cards;
      for (int i5 = 0; i5 < hand_player_id.size(); ++i5) {
        if (hand_player_id[i5] == player_up) {
          player_cards.push_back(hand_card_value[i5]);
        }
      }
      int player_card_number = player_cards.size() + 1;

      int card_i_wh = shuffled_deck[0];
      int card_v_wh = id_to_value_blackjack(card_i_wh);
      // Deal card to the player in the game
      game_shuffle_id.push_back(current_shuffle);
      game_hand_id.push_back(i1 + 1);
      game_player_id.push_back(player_up);
      game_card_number.push_back(player_card_number);
      game_card_id.push_back(card_i_wh);
      game_card_value.push_back(card_v_wh);
      // and in the current hand
      hand_player_id.push_back(player_up);
      hand_card_id.push_back(card_i_wh);
      hand_card_value.push_back(card_v_wh);
      // and in current player
      player_cards.push_back(card_v_wh);
      // Remove dealt card from deck
      shuffled_deck.erase(shuffled_deck.begin());

      // Score player and determine if they want to hit again
      List score_idx = score_blackjackC(wrap(player_cards));
      int score_value_idx = score_idx["player_score"];
      int soft_ace_idx = score_idx["soft_value"];
      if (!((score_value_idx < 17) | (score_value_idx == 17 & soft_ace_idx == true))) {
        idxs_to_hit.erase(idxs_to_hit.begin());
        if (idxs_to_hit.size() < 1) {
          any_to_hit = false;
        }
      }
    }
  }

  return Rcpp::DataFrame::create(
    Rcpp::Named("shuffle_id") = game_shuffle_id,
    Rcpp::Named("hand_id") = game_hand_id,
    Rcpp::Named("player_id") = game_player_id,
    Rcpp::Named("card_number") = game_card_number,
    Rcpp::Named("card_id") = game_card_id,
    Rcpp::Named("card_value") = game_card_value
  );
}

/*** R
cards <- as.integer(paste(rep(1:4, each = 13), rep(sprintf("%02.f", c(14, 2:13)), times = 4), sep = ""))
simulate_blackjackC(5, 10, cards)
*/
