#' @importFrom R6 R6Class
#' @export
blackjack <- R6Class(
  classname = "blackjack",
  public = list(
    options = NULL,
    player_hands = NULL,
    player_value = NULL,
    player_result = NULL,
    # print = function(...) {
    #   cat("Game of blackjack")
    #   invisible(self)
    # },
    initialize = function(...) { create_game_blackjack(self, private, ...) },
    deal = function() { deal_blackjack(self, private) },
    hit = function(player_id) { hit_blackjack(self, private, player_id) },
    stand = function() { stand_blackjack(self, private) }
  ),
  private = list(
    deck = NULL,
    dealer_hand = NULL,
    dealer_value = NULL
  )
)