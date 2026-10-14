// engineLoadEnvTex at 0x00191430, 92 bytes, in SLUS_205.65.
// Observed: the argument is a texture record. Its word at byte 16 is a packet address and its
// unsigned halfword at byte 6 is a quadword count. The sequence is: sceDmaGetChan(2); store the
// halfword (engineEnvTexAddress >> 8, arithmetic shift of the word at 0x004844d0) at the
// packet address OR 0x20000000 plus 20; a sync instruction; sceDmaSendN(channel, packet, count);
// sceGsSyncPath(0, 0). Nothing is returned. The channel call comes first, the halfword store
// after it.
// Inferred: 0x20000000 is the uncached alias, as in engineRestore. Unknown: the record layout
// other than these two fields, and why the address is shifted by 8.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store16(unsigned address, unsigned value);
extern "C" void norrath_host_sync();
#define NORRATH_STORE16(address, value) norrath_host_store16((address), (value))
#define NORRATH_SYNC() norrath_host_sync()
#else
#define NORRATH_STORE16(address, value) \
    (*reinterpret_cast<volatile unsigned short*>((address)) = static_cast<unsigned short>(value))
#define NORRATH_SYNC() asm volatile("sync" ::: "memory")
#endif

extern "C" int engineEnvTexAddress;
extern "C" void* sceDmaGetChan(int channel);
extern "C" void sceDmaSendN(void* channel, void* packet, unsigned count);
extern "C" void sceGsSyncPath(int mode, int timeout);

// Only the two observed fields are named; the rest of the record is unknown.
struct EnvTextureRecord {
    unsigned char unknown0[6];
    unsigned short quadwordCount;
    unsigned char unknown8[8];
    unsigned packetAddress;
};

extern "C" void engineLoadEnvTex__FP8_texture(EnvTextureRecord* texture) {
    void* channel = sceDmaGetChan(2);
    unsigned packet = texture->packetAddress;
    NORRATH_STORE16((packet | 0x20000000u) + 20u, engineEnvTexAddress >> 8);
    NORRATH_SYNC();
    sceDmaSendN(channel, reinterpret_cast<void*>(static_cast<unsigned long>(packet)),
                texture->quadwordCount);
    sceGsSyncPath(0, 0);
}
