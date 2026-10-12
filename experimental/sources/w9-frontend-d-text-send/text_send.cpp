// Observed: outMsg is a byte buffer whose byte 0 is a length, byte 1 a
// position and bytes from offset 3 the text. textSend first moves the bytes
// that follow offset 3 + position down one place (length - position bytes),
// sets the position byte to 0xff and decrements the length byte. If any of
// the remaining length bytes of text is not a space by the _ctype_ table (bit
// 8), it calls netSendMessage(dataHandle, outMsg, length + 5) and stores 1 in
// waiting. Otherwise it resets the message to length 1, text byte 0x20, a
// zero at offset 4 and position 0, and sends nothing.
// Inferred: none.
// Unknown: the message layout beyond these bytes, the meaning of byte 2, and
// the capacity of outMsg.

extern "C" unsigned char outMsg[];
extern "C" const unsigned char _ctype_[];
extern "C" int dataHandle;
extern "C" int waiting;
extern "C" void* memmove(void* destination, const void* source, __SIZE_TYPE__ length);
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* message, int length);

extern "C" void textSend__Fv() {
    unsigned char length = outMsg[0];
    unsigned char position = outMsg[1];
    memmove(outMsg + position + 3, outMsg + position + 4, length - position);
    outMsg[1] = 0xff;
    outMsg[0] = (unsigned char)(outMsg[0] - 1);
    unsigned char count = outMsg[0];
    int hasText = 0;
    for (int i = 0; i < count; ++i) {
        if ((_ctype_ + 1)[outMsg[3 + i]] & 8) {
            continue;
        }
        hasText = 1;
    }
    if (hasText) {
        netSendMessage__FiPUci(dataHandle, outMsg, outMsg[0] + 5);
        waiting = 1;
    } else {
        outMsg[0] = 1;
        outMsg[3] = 0x20;
        outMsg[4] = 0;
        outMsg[1] = 0;
    }
}
