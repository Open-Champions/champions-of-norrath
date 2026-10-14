// Observed: playerPrintPaperDoll__Fi at 0x001ac258 is 8 bytes: a return with an empty delay slot.
// It takes one int argument (a caller passes a player number) and does nothing with it.
// Unknown: whether the body was removed from the shipped build; no other behaviour is observed.

extern "C" void playerPrintPaperDoll__Fi(int) {}
