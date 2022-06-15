check_options_blackjack <- function(number_of_decks = 1, number_of_players = 1, player_seat = 1, ...) {
  ### Set defaults of options not specified ###
  # Must have at least 1 deck
  if (number_of_decks < 1) {
    warning("The number of decks must be at least 1. Using the default value of 1.")
  }
  number_of_decks <- max(1, number_of_decks)

  # Number of players must greater than 0 and cannot exceed 7 (which includes a spot for the player)
  if (number_of_players < 1 | number_of_players > 7) {
    warning("The number of players must be between 1-7. Setting number_of_players to the closest valid value.")
  }
  number_of_players <- max(1, min(7, number_of_players))

  # Player seat must be between 1 and the number of players
  if (player_seat < 1 | player_seat > number_of_players) {
    warning("The player's seat must be between 1 and the number of players. Setting player_seat to the closest valid value.")
  }
  player_seat <- max(1, min(number_of_players, player_seat))

  # Final options list
  ops <- list(
    number_of_decks = number_of_decks,
    number_of_players = number_of_players,
    player_seat = player_seat
  )

  return(ops)
}
