deal_blackjack <- function(self, private) {
  # Deal the dealer's cards (accounts for deal order wrt players)
  dealer_position <- self$options$number_of_players + 1
  private$dealer_hand <- private$deck[c(dealer_position, dealer_position*2)]
  private$deck <- private$deck[-c(dealer_position, dealer_position*2)]

  # Deal the player's cards
  self$player_hands[1:(2*self$options$number_of_players)] <- private$deck[1:(2*self$options$number_of_players)]
  private$deck <- private$deck[-(1:(2*self$options$number_of_players))]

  # Update player status
  self$player_value <- score_blackjack(self$player_hands)
  self$player_result <- results_blackjack(score = self$player_value, hands = self$player_hands)

  invisible(self)
}