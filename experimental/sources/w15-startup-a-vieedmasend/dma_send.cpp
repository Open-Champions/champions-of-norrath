// Observed: VIEEDMASend__FiPv at 0x010f9a40 is 116 bytes. It calls sceDmaGetChan with its
// first argument, then, only when that argument equals 1, stores the word 2 at the
// hardware address 0x1000e010 (the DMAC status register). It then returns the result of
// sceDmaSend(channel_pointer, second_argument); the channel-1 path reaches it as a tail
// call, the other as a normal call with identical arguments.
// Inferred: the store acknowledges the channel-1 completion bit before a new transfer is
// started; channel 1 is the VIF1 channel in the SDK numbering.
// Unknown: why only channel 1 needs it, and the second argument's type (a DMA tag chain
// pointer in the callers seen so far).
#ifndef NORRATH_DMAC_STAT
#define NORRATH_DMAC_STAT (*reinterpret_cast<volatile unsigned*>(0x1000e010))
#endif
extern "C" {
struct sceDmaChan;
sceDmaChan* sceDmaGetChan(int channel);
int sceDmaSend(sceDmaChan* channel, void* tag);

int VIEEDMASend__FiPv(int channel, void* data) {
    sceDmaChan* chan = sceDmaGetChan(channel);
    if (channel == 1) NORRATH_DMAC_STAT = 2;
    return sceDmaSend(chan, data);
}
}
