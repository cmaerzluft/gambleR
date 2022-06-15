# To Do:
# Add function to check game options are correct for the created game
#   e.g. check_opts(game) -> check_opts_blackjack, check_opts_texasholdem
# Add function to add player information to
# Add function to add game functions to the public list
#   e.g. add_funcs(game) -> add_funcs_blackjack -> self$deal <- function() ... and self$hit <- function()
new_game <- function(self, game, ...) {
  # Check game options
  game_opts <- list(...)


  self$game <- game
  self$options <- game_opts

  # Add game specific functions to public

  invisible(self)
}