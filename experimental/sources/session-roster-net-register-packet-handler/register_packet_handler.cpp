// Observed netRegisterPacketHandler__FPFPUci_v at 0x00135068, 160 bytes with
// padding. Same search as the message registrar over pktHandler (0x004f3f10),
// but with four slots. A hit returns its index; otherwise the first empty slot
// takes the callback. With all four used the loop ends at index 4, the word
// after the table, which is stored to and returned. In the image that word is
// the first word of scriptParticle0 (0x004f3f20). The array is declared without
// a bound so that store is not assumed away.
typedef void (*NetHandler)(unsigned char*, int);
extern "C" NetHandler pktHandler[];

extern "C" int netRegisterPacketHandler__FPFPUci_v(NetHandler handler) {
    int index = 0;
    for (; index < 4; ++index) {
        if (pktHandler[index] == handler) return index;
    }
    index = 0;
    while (pktHandler[index] != 0) {
        ++index;
        if (index >= 4) break;
    }
    pktHandler[index] = handler;
    return index;
}
