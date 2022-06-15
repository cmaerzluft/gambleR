results_blackjack <- function(score, hands, dealer_score = NULL, dealer_hand = NULL) {
  result <- ifelse(
    score > 21, "Busted", ifelse(
      rowSums(hands > 0) > 0 & score == 21, "Blackjack",
      score
    ))

  special_cases <- c("Busted", "Blackjack")
  if (!is.null(dealer_score) & !is.null(dealer_hand)) {
    dealer_result <- results_blackjack(dealer_score, matrix(dealer_hand, nrow = 1))
    result <- ifelse(
      !(result %in% special_cases) & !(dealer_result %in% special_cases), ifelse(
        result == dealer_result, "Push", ifelse(
          score > dealer_score, "Win", ifelse(
            score < dealer_score, "Lose"
          ))),
      result
    )
  }

  return(result)
}