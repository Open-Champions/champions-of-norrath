// Observed msgHudRecall__Fc at 0x001c7900, 48 bytes.
// It builds a two-byte message, 3 then the argument byte, and sends it with
// netSendMessage__FiPUci on the handle word hudMsgHandle (0x00486aec). The
// frame holds a 400-byte message buffer; only the first two bytes are written.
// The send routine and the handle are not defined here.
extern "C" int hudMsgHandle;
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);

extern "C" void msgHudRecall__Fc(char recall) {
    unsigned char message[400];
    message[0] = 3;
    message[1] = static_cast<unsigned char>(recall);
    netSendMessage__FiPUci(hudMsgHandle, message, 2);
}
