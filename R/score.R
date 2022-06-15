#' Score Blackjack
#'
#' @param game data.frame(1) or numeric(n). If a data.frame is provided, it must contain columns card_value and player_id.
#' The data.frame should be in long format where a row represents one card in a players hand. If a numeric vector is
#' provided it should just be the card value. See Details.
#'
#' @details
#' The card value should be a three digit number code where the first digit represents the suit (1, 2, 3, or 4) and the
#' last two digits represent the card code (02-14). Values 11-13 represent the face cards (Jack, Queen, King) while 14
#' represents the Ace. Face cards are automatically converted to be worth 10 points. The Ace is counted as a 1 if
#' treating it as an 11 would result in the player busting (exceeding a score of 21), otherwise it is treated as an 11.
#'
#' @return list().
#' player_score = numeric. the players score.
#' soft_value = logical. The player has a soft ace (ace treated as 11).
#'
#' @export
score_blackjack <- function(game) {
  UseMethod("score_blackjack")
}

#' @export
score_blackjack.data.frame <- function(game) {
  value <- game$card_value %% 100
  value[value %in% 11:13] <- 10
  value[value == 14] <- 1
  has_ace <- tapply(X = value, INDEX = game$player_id, FUN = function(x) sum(x == 1) > 0)
  player_score <- tapply(X = value, INDEX = game$player_id, FUN = sum)
  soft_value <- has_ace & (player_score + 10) <= 21
  player_score <- player_score + 10*soft_value

  return(list(player_score = player_score, soft_value = soft_value))
}

#' @export
score_blackjack.numeric <- function(game) {
  value <- game %% 100
  value[value %in% 11:13] <- 10
  value[value == 14] <- 1
  has_ace <- sum(value == 1) > 0
  player_score <- sum(value)
  soft_value <- has_ace & (player_score + 10) <= 21
  player_score <- player_score + 10*soft_value

  return(list(player_score = player_score, soft_value = soft_value))
}