shuffle_blackjack <- function(number_of_decks) {
  # Completely shuffle the deck
  shuf <- sample(rep(gambleR::card_deck$card_id, number_of_decks), size = 52*number_of_decks)

  # Burn one card on return
  return(shuf[-1])
}