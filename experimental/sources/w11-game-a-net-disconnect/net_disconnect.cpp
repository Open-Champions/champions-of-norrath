// Observed netDisconnect__Fv at 0x001351c0, 120 bytes. For each of the four player numbers 0..3 in
// order: when netPlayerIsLocal__Fi says the player is not local (zero result) and the number is
// below nmPartners (signed compare), the byte at offset 17621 of that player's 17632-byte record in
// `partners` is set to 1. Afterwards the byte totallyDisconnected is set to 1. No arguments; the
// image leaves 1 in the result register as a side effect of the final store, so the result is
// treated as void here.
// Inferred: byte 17621 marks a remote partner as gone (advanceRealState skips partners whose byte
// 17621 is nonzero). Unknown: what clears the marks again. Nothing here ran on the EE.
extern "C" {
extern int nmPartners;
extern unsigned char partners[];
extern signed char totallyDisconnected;
int netPlayerIsLocal__Fi(int player);
}

extern "C" void netDisconnect__Fv() {
    for (int i = 0; i < 4; ++i) {
        if (netPlayerIsLocal__Fi(i) == 0 && i < nmPartners) partners[i * 17632 + 17621] = 1;
    }
    totallyDisconnected = 1;
}
