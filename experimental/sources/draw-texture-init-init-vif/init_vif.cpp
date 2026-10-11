// Observed initVIF__Fv at 0x0018f118, 224 bytes. It writes 1024 through ctc2
// to COP2 register 28, sends the word at 0x0031b790 on dmaVif1 and the word
// at 0x0031b510 on dmaVif0, and waits for both with mode 0 and timeout 0.
// Those two addresses are loadVU1Program and loadVU0Program. FlushCache(0)
// and sceGsSyncPath(0, 0) follow. 2 is stored at 0x10003820 and 0x10003c20,
// then sceGsSyncV(0) runs twice. The last stores are 0x30ae4000 at
// 0x1100ffb4, 0x30be4000 at 0x1100ffb8, 0x8000 at 0x1100ff60, and zero at
// 0x1100ff64, 0x1100ff68, and 0x1100ff6c. dmaVif0 and dmaVif1 are reloaded
// for each call; these are the words at 0x00486a40 and 0x00486a44.
// Identifying the 0x10003xxx words as VIF registers is inferred from the
// public EE map. Fresh compilation is not an original-byte match.

extern "C" {
unsigned* dmaVif0;
unsigned* dmaVif1;
void sceDmaSend(unsigned* channel, void* packet);
void sceDmaSync(unsigned* channel, int mode, int timeout);
void FlushCache(int mode);
void sceGsSyncPath(int mode, int timeout);
void sceGsSyncV(int mode);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_vif_ctc2(unsigned value);
extern "C" void norrath_store32(unsigned address, unsigned value);
#else
static inline void norrath_vif_ctc2(unsigned value) {
    __asm__ volatile("ctc2 %0, $28" : : "r"(value));
}
static inline void norrath_store32(unsigned address, unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address)) = value;
}
#endif

extern "C" void initVIF__Fv() {
    norrath_vif_ctc2(1024u);
    unsigned* channel = dmaVif1;
    sceDmaSend(channel, reinterpret_cast<void*>(static_cast<unsigned long>(0x0031b790u)));
    channel = dmaVif1;
    sceDmaSync(channel, 0, 0);
    channel = dmaVif0;
    sceDmaSend(channel, reinterpret_cast<void*>(static_cast<unsigned long>(0x0031b510u)));
    channel = dmaVif0;
    sceDmaSync(channel, 0, 0);
    FlushCache(0);
    sceGsSyncPath(0, 0);
    norrath_store32(0x10003820u, 2u);
    norrath_store32(0x10003c20u, 2u);
    sceGsSyncV(0);
    sceGsSyncV(0);
    norrath_store32(0x1100ffb4u, 0x30ae4000u);
    norrath_store32(0x1100ffb8u, 0x30be4000u);
    norrath_store32(0x1100ff60u, 0x8000u);
    norrath_store32(0x1100ff64u, 0u);
    norrath_store32(0x1100ff68u, 0u);
    norrath_store32(0x1100ff6cu, 0u);
}
