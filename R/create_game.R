create_game_blackjack <- function(self, private, ...) {
  # Make sure options are correctly specified
  self$options <- do.call(check_options_blackjack, list(...))

  # Seat the table
  self$player_hands <- matrix(0, nrow = self$options$number_of_players, ncol = 2)
  colnames(self$player_hands) <- c("Card 1", "Card 2")
  rownames(self$player_hands) <- paste("Player", 1:nrow(self$player_hands))
  rownames(self$player_hands)[self$options$player_seat] <- "You"

  # Shuffle the deck and burn one card
  private$deck <- shuffle_blackjack(self$options$number_of_decks)

  invisible(self)
}