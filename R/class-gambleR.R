#' @importFrom R6 R6Class
#' @export
gambleR <- R6Class(
  classname = "gambleR",
  # inherit = blackjack,
  public = list(
    game = NULL,
    options = NULL,
    initialize = function(game, ...) { new_game(self, game = game, ...) }
  )
)