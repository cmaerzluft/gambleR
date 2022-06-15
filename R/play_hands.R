play_hands <- function(games) {

  cards <- rep(gambleR::card_deck$card_id, n_decks)
  player_ids <- c(1:n_players, 0)
  n_players_dealer <- n_players + 1
  empty_col <- rep(NA_integer_, n_players_dealer*n_hands)
  players <- data.frame(
    hand_id = rep(1:n_hands, each = n_players_dealer),
    player_id = rep(player_ids, n_hands),
    card1 = empty_col,
    card2 = empty_col
  )

  # Initialize info used multiple times
  playersx2 <- n_players_dealer*2

  # Samples all cards needed
  dealt_cards <- replicate(n_hands, sample(cards, playersx2), simplify = TRUE)

  # Pull pocket cards
  dealt_cards <- matrix(dealt_cards, ncol = 2, byrow = TRUE)
  players[, c("card1", "card2")] <- dealt_cards

}