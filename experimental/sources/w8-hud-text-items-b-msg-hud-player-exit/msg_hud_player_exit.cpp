// Observed msgHudPlayerExit__Fv at 0x001c7970, 144 bytes.
// Nothing happens unless the signed byte netNetworkMode (0x00483829) is
// nonzero and nmPlayers (0x00484538) is at least 2. The routine then looks for
// the first player index below nmPlayers for which netPlayerIsLocal__Fi returns
// nonzero, using the index equal to nmPlayers when none does. It sends a
// two-byte message, 5 then that index, on hudMsgHandle with netSendMessage__FiPUci.
// nmPlayers is re-read each pass. The frame holds a 400-byte buffer. The
// callees and the three words are not defined here.
extern "C" signed char netNetworkMode;
extern "C" int nmPlayers;
extern "C" int hudMsgHandle;
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);

extern "C" void msgHudPlayerExit__Fv() {
    if (netNetworkMode == 0) return;
    if (nmPlayers < 2) return;
    unsigned char message[400];
    unsigned char* cursor = message;
    int player;
    for (player = 0; player < nmPlayers; ++player) {
        if (netPlayerIsLocal__Fi(player) != 0) break;
    }
    *cursor++ = 5;
    *cursor++ = static_cast<unsigned char>(player);
    netSendMessage__FiPUci(hudMsgHandle, message, static_cast<int>(cursor - message));
}
