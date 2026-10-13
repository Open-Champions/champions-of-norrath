// Observed msgInventoryExit__Fiii at 0x001b17d0, 56 bytes.
// It builds a four-byte message in a 400-byte frame buffer: byte 0 is 4, bytes
// 1 to 3 are the low bytes of the three arguments in order. It sends it with
// netSendMessage__FiPUci on the handle word invMsgHandle (0x00486a88), size 4.
// Unknown: what the three argument bytes mean to the receiver.
extern "C" int invMsgHandle;
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);

extern "C" void msgInventoryExit__Fiii(int a, int b, int c) {
    unsigned char message[400];
    message[0] = 4;
    message[1] = static_cast<unsigned char>(a);
    message[2] = static_cast<unsigned char>(b);
    message[3] = static_cast<unsigned char>(c);
    netSendMessage__FiPUci(invMsgHandle, message, 4);
}
