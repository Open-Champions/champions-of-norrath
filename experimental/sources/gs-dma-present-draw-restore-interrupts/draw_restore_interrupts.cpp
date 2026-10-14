/* Observed drawRestoreInterrupts__Fv at 0x0015f928, 280 bytes. Channels 0
 * and 1 from sceDmaGetChan have bit 0x40 set. Channel 2 has that bit
 * cleared. Each change is followed by FlushCache(0). The pointers are
 * stored at the local words 0x004868d8, 0x004868dc, and 0x004868d4. Those
 * are not the globals dmaVif0, dmaVif1, and dmaGif. No instruction in the
 * image loads the three locals. drawInit keeps private copies, so these
 * stores use this unit's own words. reloadDMA__Fi is registered on channels
 * 0, 1, and 2 and the ids are stored in vif0HID, vif1HID, and gifHID.
 * gsIntHandler__Fi is registered on cause 0 and that id is discarded.
 * 32000 is stored at 0x12001010 and 4 at 0x10003010. Fresh compilation is
 * not an original-byte match.
 */
extern "C" int vif0HID;
extern "C" int vif1HID;
extern "C" int gifHID;
extern "C" unsigned* sceDmaGetChan(int channel);
extern "C" void FlushCache(int mode);
extern "C" int AddDmacHandler(int channel, void (*handler)(int), int next);
extern "C" void EnableDmac(int channel);
extern "C" int AddIntcHandler(int cause, int (*handler)(int), int next);
extern "C" void EnableIntc(int cause);
extern "C" void reloadDMA__Fi(int cause);
extern "C" int gsIntHandler__Fi(int cause);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_store64(unsigned address, unsigned long long value);
extern "C" void norrath_store32(unsigned address, unsigned value);
extern "C" unsigned* norrath_restored_vif0();
extern "C" unsigned* norrath_restored_vif1();
extern "C" unsigned* norrath_restored_gif();
#else
static inline void norrath_store64(unsigned address, unsigned long long value) {
    *reinterpret_cast<volatile unsigned long long*>(static_cast<unsigned long>(address)) = value;
}
static inline void norrath_store32(unsigned address, unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address)) = value;
}
#endif

static unsigned* restored_vif0;
static unsigned* restored_vif1;
static unsigned* restored_gif;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned* norrath_restored_vif0() { return restored_vif0; }
extern "C" unsigned* norrath_restored_vif1() { return restored_vif1; }
extern "C" unsigned* norrath_restored_gif() { return restored_gif; }
#endif

extern "C" void drawRestoreInterrupts__Fv() {
    unsigned* channel = sceDmaGetChan(0);
    channel[0] |= 0x40u;
    restored_vif0 = channel;
    FlushCache(0);
    channel = sceDmaGetChan(1);
    channel[0] |= 0x40u;
    restored_vif1 = channel;
    FlushCache(0);
    channel = sceDmaGetChan(2);
    channel[0] &= 0xffffffbfu;
    restored_gif = channel;
    FlushCache(0);
    vif0HID = AddDmacHandler(0, reloadDMA__Fi, 0);
    EnableDmac(0);
    vif1HID = AddDmacHandler(1, reloadDMA__Fi, 0);
    EnableDmac(1);
    gifHID = AddDmacHandler(2, reloadDMA__Fi, 0);
    EnableDmac(2);
    AddIntcHandler(0, gsIntHandler__Fi, 0);
    EnableIntc(0);
    norrath_store64(0x12001010u, 32000ull);
    norrath_store32(0x10003010u, 4u);
}
