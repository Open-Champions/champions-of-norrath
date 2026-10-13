// Observed playerLogInventory__Fi at 0x001ac490, 96 bytes.
// Argument: a player number. Walks the three 4616-byte lists at record+328 of
// the 23984-byte player record (the count is the signed word at list+4608) and
// visits each slot, but the loop body is empty: nothing is read from a slot,
// nothing is written and nothing is called. Observed: the image keeps the
// iteration (with padding where a call would sit) and returns nothing.
// Inferred: a logging call was compiled out; no message text is recorded here.
extern "C" void playerLogInventory__Fi(int owner) {
    (void)owner;
}
