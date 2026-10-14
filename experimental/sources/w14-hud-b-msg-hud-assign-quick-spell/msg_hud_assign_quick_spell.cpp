// Observed msgHudAssignQuickSpell__Fiii at 0x001c7858, 52 bytes, 416-byte frame. It builds a
// four-byte message in a local buffer: byte 0 is zero and bytes 1, 2 and 3 are the low bytes of the
// three integer arguments in order. It then calls netSendMessage__FiPUci(hudMsgHandle, buffer, 4)
// and returns nothing. hudMsgHandle is a gp-relative word read at call time.
// Inferred: a message type of zero means "assign quick spell" and the arguments are small indices
// (the names come from the symbol table). Unknown: the receiving handler, and the meaning of each
// byte; the rest of the original's large local buffer is not touched.
extern "C" {
extern int hudMsgHandle;
void netSendMessage__FiPUci(int handle, unsigned char* data, int length);

void msgHudAssignQuickSpell__Fiii(int a, int b, int c) {
    unsigned char message[4];
    message[0] = 0;
    message[1] = static_cast<unsigned char>(a);
    message[2] = static_cast<unsigned char>(b);
    message[3] = static_cast<unsigned char>(c);
    netSendMessage__FiPUci(hudMsgHandle, message, 4);
}
}
