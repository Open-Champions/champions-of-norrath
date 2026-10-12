// Observed: multiHudAddHelpMessage__FiPUsT1i at 0x001c76a0 is 148 bytes. With
// arguments (player, title, body, side) it calls netPlayerIsLocal__Fi(player)
// and does nothing more if that is zero. Otherwise it calls
// coreInterfaceFlare__Fi(player), then: stores 1 in
// multiHelpMessageOn[player] (the byte array symbol at 0x00483f38, size 4), stores side in multiSide[player], title in
// multiTitle[player] and body in multiBody[player] (all 4-byte slots), and
// stores 0 in the byte 9488 bytes into shopList plus player.
// Inferred: multiHelpMessageOn is a per-player flag, from its name and the
// store of 1; the shopList byte is a per-player cleared state, from the store
// pattern only.
// Unknown: what shopList + 9488 holds and the units of side.

extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" void coreInterfaceFlare__Fi(int player);
extern "C" unsigned char multiHelpMessageOn[];
extern "C" int multiSide[];
extern "C" unsigned short* multiTitle[];
extern "C" unsigned short* multiBody[];
extern "C" unsigned char shopList[];

extern "C" void multiHudAddHelpMessage__FiPUsT1i(int player, unsigned short* title,
                                                 unsigned short* body, int side) {
    if (netPlayerIsLocal__Fi(player) == 0) return;
    coreInterfaceFlare__Fi(player);
    multiHelpMessageOn[player] = 1;
    multiSide[player] = side;
    multiTitle[player] = title;
    multiBody[player] = body;
    shopList[9488 + player] = 0;
}
