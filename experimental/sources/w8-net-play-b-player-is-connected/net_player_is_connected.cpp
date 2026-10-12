// Observed netPlayerIsConnected at 0x00135150, 60 bytes. A zero netNetworkMode
// byte returns 1 without looking at the argument. Otherwise an argument not
// below nmPartners (signed compare) returns 0, and any other argument returns
// whether the signed byte at offset 17621 of that player's 17632-byte partner
// record is zero.
// Inferred: the byte at 17621 is the "disconnected" mark that
// netDisconnectPlayer sets; the name of the function is the image's own.
// Unknown: what else writes the byte; a negative argument is not rejected.
extern "C" {
extern signed char netNetworkMode;
extern int nmPartners;
extern unsigned char partners[];
}

extern "C" int netPlayerIsConnected__Fi(int player) {
    if (netNetworkMode == 0) return 1;
    if (!(player < nmPartners)) return 0;
    return static_cast<signed char>(partners[player * 17632 + 17621]) == 0;
}
