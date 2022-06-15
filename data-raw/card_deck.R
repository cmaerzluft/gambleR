# Define Single Card deck
card_deck <- data.frame(
  suit = rep(c("Spades", "Hearts", "Clubs", "Diamonds"), each = 13),
  value = rep(c("Ace", 2:10, "Jack", "Queen", "King"), times = 4),
  suit_id = rep(1L:4L, each = 13),
  value_id = rep(c(14L, 2L:13L), times = 4),
  card_id = as.integer(paste(rep(1:4, each = 13), rep(sprintf("%02.f", c(14, 2:13)), times = 4), sep = ""))
)
usethis::use_data(card_deck, overwrite = TRUE)