// Observed textHandler__FPUci at 0x002ca8d0, 488 bytes. Received text
// messages are kept in inMsg (0x008d8938, 1008 bytes = eight 126-byte slots)
// with eight words in msgTimeout (0x008d8d28, 32 bytes) beside them. A
// message first shifts every slot, and its word, up one place from index 6
// down to index 0 (the old last one is lost). The new message (size bytes,
// unbounded) is then copied into slot 0 and msgTimeout[0] is set to zero. If
// slot 0 equals the 128-byte buffer outMsg (0x008d88b8) -- the bytes at 0, 1
// and 2 are equal and the span from byte 3 for slot byte 0 plus one bytes
// compares equal -- outMsg is reset: byte 0 becomes 1, byte 3 becomes 32,
// byte 4 and byte 1 become 0, and the word waiting (0x00486dac) is cleared.
// The field roles are not established; the names are the ELF symbol names.
extern "C" {
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);
int memcmp(const void* left, const void* right, __SIZE_TYPE__ count);

struct NorrathTextSlot {
    unsigned char bytes[126];
};
extern NorrathTextSlot inMsg[8];
extern int msgTimeout[8];
extern unsigned char outMsg[128];
extern int waiting;

void textHandler__FPUci(unsigned char* message, int size) {
    for (int index = 7; index > 0; --index) {
        inMsg[index] = inMsg[index - 1];
        msgTimeout[index] = msgTimeout[index - 1];
    }
    memcpy(inMsg[0].bytes, message, static_cast<__SIZE_TYPE__>(size));
    const unsigned char* head = inMsg[0].bytes;
    bool same = false;
    if (static_cast<signed char>(head[2]) == static_cast<signed char>(outMsg[2]) &&
        head[0] == outMsg[0] && head[1] == outMsg[1]) {
        same = memcmp(head + 3, outMsg + 3, static_cast<__SIZE_TYPE__>(head[0]) + 1) == 0;
    }
    if (same) {
        outMsg[0] = 1;
        outMsg[3] = 32;
        outMsg[4] = 0;
        waiting = 0;
        outMsg[1] = 0;
    }
    msgTimeout[0] = 0;
}
}
