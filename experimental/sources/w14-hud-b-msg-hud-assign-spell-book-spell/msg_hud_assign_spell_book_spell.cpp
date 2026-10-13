// Observed msgHudAssignSpellBookSpell__Fiiil at 0x001c7ab8, 64 bytes, 416-byte frame. It fills a
// local buffer: byte 0 is 1, bytes 1 and 2 are the low bytes of the first two integer arguments,
// byte 3 is zero, the word at offset 4 is the third argument and the doubleword at offset 8 is the
// fourth (stored with a 64-bit store, so the fourth parameter is 64 bits wide). It then calls
// netSendMessage__FiPUci(hudMsgHandle, buffer, 16) and returns nothing. All 16 bytes are defined.
// Inferred: type 1 follows the type-0 quick-spell message, so it is a different message kind.
// Unknown: the meaning of the fields, and whether the last argument is a pointer or an integer.
extern "C" {
extern int hudMsgHandle;
void netSendMessage__FiPUci(int handle, unsigned char* data, int length);

void msgHudAssignSpellBookSpell__Fiiil(int a, int b, int c, long long d) {
    unsigned char message[16];
    message[0] = 1;
    message[1] = static_cast<unsigned char>(a);
    message[2] = static_cast<unsigned char>(b);
    message[3] = 0;
    *reinterpret_cast<int*>(message + 4) = c;
    *reinterpret_cast<long long*>(message + 8) = d;
    netSendMessage__FiPUci(hudMsgHandle, message, 16);
}
}
