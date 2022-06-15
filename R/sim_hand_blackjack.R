sim_hand_blackjack <- function(game, cards, hand_id, player_ids, n_players_dealer) {
  # First deal
  deal <- sample(length(cards), n_players_dealer*2)
  game$card_value <- cards[deal]
  remaining_cards <- cards[-deal]

  # Score (to help determine hit strategy)
  scores <- score_blackjack(game = game)
  to_hit <- scores$player_score < 17 | (scores$player_score == 17 & scores$soft_value)

  # Stop game if deal has 21 at start
  if (scores$player_score[length(scores$player_score)] == 21) {
    return(list(game = game, cards = remaining_cards))

  } else {
    # iterate over players in the hand, hitting based on hit strategy
    while (any(to_hit)) {
      # Error check
      if (length(remaining_cards) == 0) { stop(
        "Not enough cards. Change 4 to a larger number in length(res$cards) < n_players_dealer*4 in simulate_blackjack.R"
      ) }

      # Get player's hand that needs to be hit on
      player_up <- as.integer(names(which.max(to_hit)))
      player_hand <- game$card_value[game$player_id == player_up] %% 100

      # Deal a card from the deck
      deal_card_idx <- sample(length(remaining_cards), 1)
      game[nrow(game) + 1, ] <- c(hand_id, player_up, length(player_hand) + 1L, remaining_cards[deal_card_idx])
      remaining_cards <- remaining_cards[-deal_card_idx]

      # Score and decide whether to hit again
      scores <- score_blackjack(game = game$card_value[game$player_id == player_up])
      to_hit[names(to_hit) == player_up] <- scores$player_score < 17 | (scores$player_score == 17 & scores$soft_value)
    }
  }

  return(list(game = game, cards = remaining_cards))
}
