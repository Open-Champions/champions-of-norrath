// Observed msgHudSavePoint__FP9Savepoint at 0x001c7930, 60 bytes.
// It builds an eight-byte message in a 400-byte frame buffer: bytes 0 to 3 are
// all 4, and the word at offset 4 is the argument pointer (a 32-bit value on the
// target). It sends it with netSendMessage__FiPUci on the handle word
// hudMsgHandle (0x00486aec), size 8.
// Inferred: the four 4 bytes are written one at a time, not as a word.
// Unknown: why the first four bytes repeat the value 4, and what the receiver
// does with the pointer.
extern "C" int hudMsgHandle;
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);

extern "C" void msgHudSavePoint__FP9Savepoint(void* savepoint) {
    unsigned char message[400];
    message[0] = 4;
    message[1] = 4;
    message[2] = 4;
    message[3] = 4;
    *reinterpret_cast<void**>(message + 4) = savepoint;
    netSendMessage__FiPUci(hudMsgHandle, message, 8);
}
