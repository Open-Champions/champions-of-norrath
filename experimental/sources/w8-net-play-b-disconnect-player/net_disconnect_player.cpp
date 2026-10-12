// Observed netDisconnectPlayer at 0x00135190, 44 bytes. When the argument is
// below nmPartners (signed compare) the byte at offset 17621 of that player's
// 17632-byte partner record is set to 1. Otherwise nothing is written.
// netPlayerIsConnected reads the same byte.
// Inferred: none. Unknown: a negative argument is not rejected by the image.
extern "C" {
extern int nmPartners;
extern unsigned char partners[];
}

extern "C" void netDisconnectPlayer__Fi(int player) {
    if (player < nmPartners) partners[player * 17632 + 17621] = 1;
}
