#include <numeric>
// CM NOTE: When deck_class is completed, can remove deck_utils since it just includes the shuffle_deck function
#include "deck_utils.h"
#include "deck_class.h"
#include "blackjack_helper_funs.h"
using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::DataFrame simulate_blackjackC(
    double n_players, double n_hands,
    std::vector<int> card_deck,
    std::string count_method = "hi-lo"
) {
  // CM NOTE: Where is a better place to put check functions?
  if (n_players < 1) { stop("Need more players."); }
  // Initialize player IDs
  int n_players_dealer = n_players + 1;
  std::vector<int> player_ids = init_player_ids(n_players_dealer);

  // Initialize all simulated games information
  std::vector<int> game_shuffle_id;    game_shuffle_id.reserve(n_hands*n_players_dealer*2);
  std::vector<int> game_hand_id;       game_hand_id.reserve(n_hands*n_players_dealer*2);
  std::vector<int> game_player_id;     game_player_id.reserve(n_hands*n_players_dealer*2);
  std::vector<int> game_card_number;   game_card_number.reserve(n_hands*n_players_dealer*2);
  std::vector<int> game_card_id;       game_card_id.reserve(n_hands*n_players_dealer*2);
  std::vector<int> game_card_value;    game_card_value.reserve(n_hands*n_players_dealer*2);
  std::vector<int> game_running_score; game_running_score.reserve(n_hands*n_players_dealer*2);
  std::vector<int> game_deck_count;    game_deck_count.reserve(n_hands*n_players_dealer*2);

  // Shuffle and burn one card
  Deck table_deck;
  // CM NOTE: Deck table_deck; is identical to the next two lines of code
  std::vector<int> shuffled_deck = sample(card_deck);
  shuffled_deck.erase(shuffled_deck.begin());
  int current_shuffle = 1;
  bool new_deal = true;

  // std::vector<int> test_deck = table_deck.get_currentdeck();
  // std::cout << "The value of v : ";
  // for (int i = 0; i < test_deck.size(); i++) {
  //   std::cout << test_deck.at(i) << ' ';
  // }

  // Simulate hands
  for (int hand_iter_i1 = 0; hand_iter_i1 < n_hands; hand_iter_i1++) {
    // Individual hand information
    // CM NOTE: Doing this so we can pass this information as needed later on, but is there a fast way to use subsets
    //  of a vector because hand_ objects are really just subsets of game_ objects which is what we return ultimately
    std::vector<int> hand_player_id;     hand_player_id.reserve(n_players_dealer*2);
    std::vector<int> hand_card_number;   hand_card_number.reserve(n_players_dealer*2);
    std::vector<int> hand_card_id;       hand_card_id.reserve(n_players_dealer*2);
    std::vector<int> hand_card_value;    hand_card_value.reserve(n_players_dealer*2);

    // Reshuffle the deck if we need to
    // CM NOTE: Need a better method for formalizing when to reshuffle (and put some random variation in as well)
    if (shuffled_deck.size() < n_players_dealer*(4 + 4/n_players_dealer)) {
      // CM NOTE: I think this would be an area to go OOP. shuffled_deck would be a public member, and there would be a
      //  function for dealing that did the method below (or the method in the for loop for reshuffling) and a function
      //  for dealing a card from the top.
      shuffled_deck.clear();
      std::vector<int> new_shuffle = sample(card_deck);
      shuffled_deck.insert(shuffled_deck.begin(), new_shuffle.begin(), new_shuffle.end());
      shuffled_deck.erase(shuffled_deck.begin());
      new_deal = true;
      current_shuffle += 1;
    }

    // Deal the cards
    for (int deal_iter_i2 = 0; deal_iter_i2 < 2; deal_iter_i2++) {
      for (int player_iter_i3 = 0; player_iter_i3 < n_players_dealer; player_iter_i3++) {
        // Get the dealt card
        if (shuffled_deck.size() == 0) { stop("No cards"); }
        int card_i_i3 = shuffled_deck[0];
        int card_v_i3 = id_to_value_blackjack(card_i_i3);
        int card_c_i3 = count_card(card_v_i3, count_method);
        int current_count;

        // Deal cards tracking in the game results
        game_shuffle_id.push_back(current_shuffle);
        game_hand_id.push_back(hand_iter_i1 + 1);
        game_player_id.push_back(player_ids[player_iter_i3]);
        game_card_number.push_back(deal_iter_i2 + 1);
        game_card_id.push_back(card_i_i3);
        game_card_value.push_back(card_v_i3);
        if (new_deal) {
          current_count = count_card(0, count_method, true);
          new_deal = false;
        } else {
          current_count = game_deck_count.back();
        }
        game_deck_count.push_back(current_count + card_c_i3);

        // and in the current hand
        hand_player_id.push_back(player_ids[player_iter_i3]);
        hand_card_id.push_back(card_i_i3);
        hand_card_value.push_back(card_v_i3);

        // Remove dealt card from deck
        shuffled_deck.erase(shuffled_deck.begin());
      }
    }

    // Score players and determine who wants to hit
    List scores = score_blackjack_by_group(hand_card_value, hand_player_id, player_ids);
    std::vector<int> score_value = scores["player_score"];
    std::vector<int> soft_ace = scores["soft_value"];
    game_running_score.insert(game_running_score.end(), score_value.begin(), score_value.end());
    game_running_score.insert(game_running_score.end(), score_value.begin(), score_value.end());

    // Stop this hand if the dealer has 21
    if (score_value[score_value.size() - 1] >= 21) { continue; }

    // Determine who wants to hit
    // CM NOTE: Make this a function that can take a condition in the form of a function?
    //  i.e. hit_strategy(scores, soft, f) where f = function(x) x < 17 | (x == 17 & soft)
    //  this should also be something that can be specified for each player individually
    std::vector<int> idxs_to_hit;
    bool any_to_hit;
    for (int hit_check_iter_i2 = 0; hit_check_iter_i2 < score_value.size(); hit_check_iter_i2++) {
      if (
          (score_value[hit_check_iter_i2] < 17) |
            (score_value[hit_check_iter_i2] == 17 & soft_ace[hit_check_iter_i2] == true)
      ) {
        idxs_to_hit.push_back(hit_check_iter_i2);
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
      for (int find_player_iter_i3 = 0; find_player_iter_i3 < hand_player_id.size(); find_player_iter_i3++) {
        if (hand_player_id[find_player_iter_i3] == player_up) {
          player_cards.push_back(hand_card_value[find_player_iter_i3]);
        }
      }
      int player_card_number = player_cards.size() + 1;

      // Get the dealt card
      // CM NOTE: Need to adjust card_v_wh to be 11 for Ace when it makes sense.
      if (shuffled_deck.size() == 0) { stop("No cards"); }
      int card_i_wh = shuffled_deck[0];
      int card_v_wh = id_to_value_blackjack(card_i_wh);
      int card_c_wh = count_card(card_v_wh, count_method, false);
      int current_count = game_deck_count.back();

      // Deal card to the player in the game
      game_shuffle_id.push_back(current_shuffle);
      game_hand_id.push_back(hand_iter_i1 + 1);
      game_player_id.push_back(player_up);
      game_card_number.push_back(player_card_number);
      game_card_id.push_back(card_i_wh);
      game_card_value.push_back(card_v_wh);
      game_deck_count.push_back(current_count + card_c_wh);

      // and in the current hand
      hand_player_id.push_back(player_up);
      hand_card_id.push_back(card_i_wh);
      hand_card_value.push_back(card_v_wh);

      // and in current player
      player_cards.push_back(card_v_wh);

      // Remove dealt card from deck
      shuffled_deck.erase(shuffled_deck.begin());

      // Score player and determine if they want to hit again
      List score_idx = score_blackjack(player_cards);
      int score_value_idx = score_idx["player_score"];
      int soft_ace_idx = score_idx["soft_value"];
      game_running_score.push_back(score_value_idx);
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
    Rcpp::Named("card_value") = game_card_value,
    Rcpp::Named("running_score") = game_running_score,
    Rcpp::Named("deck_count") = game_deck_count
  );
}

/*** R
cards <- as.integer(paste(rep(1:4, each = 13), rep(sprintf("%02.f", c(14, 2:13)), times = 4), sep = ""))
simulate_blackjackC(7, 100, cards)
*/
