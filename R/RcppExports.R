# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

count_cards_by_group <- function(cards_dealt, shuffle_id, method = "none") {
    .Call(`_gambleR_count_cards_by_group`, cards_dealt, shuffle_id, method)
}

score_blackjack_by_group <- function(player_hands, player_hand_ids, player_ids) {
    .Call(`_gambleR_score_blackjack_by_group`, player_hands, player_hand_ids, player_ids)
}

ids_to_value_blackjack <- function(x) {
    .Call(`_gambleR_ids_to_value_blackjack`, x)
}

simulate_blackjackC <- function(n_players, n_hands, card_deck, count_method = "hi-lo") {
    .Call(`_gambleR_simulate_blackjackC`, n_players, n_hands, card_deck, count_method)
}

