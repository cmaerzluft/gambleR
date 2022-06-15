hit_blackjack <- function(self, private, player_id) {
  # Check player has not busted or hit 21/blackjack yet
  if (self$player_result[player_id] %in% c("Busted", "21", "Blackjack")) { return(invisible(self)) }

  # Create empty card spot if one doesn't exist yet
  min_card_loc <- which.min(self$player_hands[player_id, ])
  if (self$player_hands[player_id, min_card_loc] != 0) {
    min_card_loc <- ncol(self$player_hands) + 1
    new_name <- c(colnames(self$player_hands), paste("Card", min_card_loc))
    self$player_hands <- cbind(self$player_hands, rep(0L, nrow(self$player_hands)))
    colnames(self$player_hands) <- new_name
  }

  # Update players hand and the deck
  self$player_hands[player_id, min_card_loc] <- private$deck[1]
  private$deck <- private$deck[-1]

  # Update player status
  self$player_value <- score_blackjack(self$player_hands)
  self$player_result <- results_blackjack(score = self$player_value, hands = self$player_hands)

  invisible(self)
}