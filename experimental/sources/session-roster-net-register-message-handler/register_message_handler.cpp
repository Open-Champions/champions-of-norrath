// Observed netRegisterMessageHandler__FPFPUci_v at 0x00134ec8, 160 bytes with
// padding. msgHandler (0x004f3ed0) is searched for the callback in slots 0..15;
// a hit returns that index. Otherwise the first empty slot among the 16 takes
// the callback and its index is returned. With every slot used the loop ends at
// index 16, the word after the table, which is stored to and returned. In the
// image that word is the first word of pktHandler (0x004f3f10). The array is
// declared without a bound so that store is not assumed away.
typedef void (*NetHandler)(unsigned char*, int);
extern "C" NetHandler msgHandler[];

extern "C" int netRegisterMessageHandler__FPFPUci_v(NetHandler handler) {
    int index = 0;
    for (; index < 16; ++index) {
        if (msgHandler[index] == handler) return index;
    }
    index = 0;
    while (msgHandler[index] != 0) {
        ++index;
        if (index >= 16) break;
    }
    msgHandler[index] = handler;
    return index;
}
