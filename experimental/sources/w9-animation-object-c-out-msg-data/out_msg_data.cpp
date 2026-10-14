// Zero-initialised outgoing message storage: outMsg at 0x008d88b8, 128 bytes up to the next
// symbol (inMsg at 0x008d8938). It is BSS in the original and zero here.
// Observed: the importer reads byte 0 as a length, bytes 1 and 2 as further header bytes and
// compares length + 1 bytes starting at +3 (see save-and-quests-text-handler).
// Inferred: one message record is 126 bytes, as inMsg's 1008 bytes are eight of them, so the
// last two bytes of the 128-byte extent are alignment padding. The extent here is the
// distance to the next symbol, not a claim about the record length.
// Unknown: the layout of the record beyond the first three bytes.
extern "C" {
unsigned char outMsg[128];
}
