// Observed msgHudSetPlayerBusy__Fcc at 0x001c7890, 108 bytes.
// Both arguments are reduced to their low eight bits and sign-extended.
// A zero netNetworkMode returns without a call. Any other mode prints the
// text at 0x0046a920 and sends three bytes on hudMsgHandle: 2, the player
// byte, and the busy byte. Those three symbols are not defined here.
extern "C" unsigned char netNetworkMode;
extern "C" int hudMsgHandle;
extern "C" void scePrintf(const char* text, int player, int busy);
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);

extern "C" void msgHudSetPlayerBusy__Fcc(int player, int busy) {
    const signed char player_byte = static_cast<signed char>(player);
    const signed char busy_byte = static_cast<signed char>(busy);
    if (netNetworkMode == 0) return;
    scePrintf("MSG SEND:  Set player %d to busy=%d\n", player_byte, busy_byte);
    unsigned char message[3];
    message[0] = 2;
    message[1] = static_cast<unsigned char>(player_byte);
    message[2] = static_cast<unsigned char>(busy_byte);
    netSendMessage__FiPUci(hudMsgHandle, message, 3);
}
