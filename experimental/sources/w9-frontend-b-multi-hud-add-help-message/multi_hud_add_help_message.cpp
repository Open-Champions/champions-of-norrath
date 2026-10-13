// Observed: multiHudAddHelpMessage__FiPUsT1i at 0x001c76a0 is 148 bytes. With
// arguments (player, title, body, side) it calls netPlayerIsLocal__Fi(player)
// and does nothing more if that is zero. Otherwise it calls
// coreInterfaceFlare__Fi(player), then: stores 1 in
// multiHelpMessageOn[player] (the byte array symbol at 0x00483f38, size 4), stores side in multiSide[player], title in
// multiTitle[player] and body in multiBody[player] (all 4-byte slots), and
// stores 0 in multiRelease[player] (the byte array at 0x00486ae0).
// Inferred: multiHelpMessageOn and multiRelease are per-player flags, from
// their names and the stores of 1 and 0.
// Unknown: what multiRelease means to its readers and the units of side.

extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" void coreInterfaceFlare__Fi(int player);
extern "C" unsigned char multiHelpMessageOn[];
extern "C" int multiSide[];
extern "C" unsigned short* multiTitle[];
extern "C" unsigned short* multiBody[];
extern "C" unsigned char multiRelease[];

extern "C" void multiHudAddHelpMessage__FiPUsT1i(int player, unsigned short* title,
                                                 unsigned short* body, int side) {
    if (netPlayerIsLocal__Fi(player) == 0) return;
    coreInterfaceFlare__Fi(player);
    multiHelpMessageOn[player] = 1;
    multiSide[player] = side;
    multiTitle[player] = title;
    multiBody[player] = body;
    multiRelease[player] = 0;
}
