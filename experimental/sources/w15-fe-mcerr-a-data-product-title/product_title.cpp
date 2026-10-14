// Observed: sProductTitle at 0x011afa98, 64 bytes. The image holds the 27 bytes of the text "EverQuest
// Online" + newline + "Adventures" with a terminator, then zero bytes. VIMemCardProductTitle and
// VIMemCardSetProductTitle, and the other VIMemCard functions, address it.
// Inferred: the default product title that the card layer shows in saved-file headers (the newline matches the
// two-line form that SetTitle__16VIIconDefinitionPCc accepts); it can be replaced at run time by
// VIMemCardSetProductTitle.
// Unknown: the title's maximum length (64 bytes is the object size) and every writer.
extern "C" {
char sProductTitle[64] = "EverQuest Online\nAdventures";
}
