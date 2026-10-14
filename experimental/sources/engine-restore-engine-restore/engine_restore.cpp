// Observed engineRestore at 0x001901e0, 204 bytes. It returns.
// AddIntcHandler is the symbol at 0x002d4dc0, not AddIntcHandler2.
// The three absolute stores and the sync are the instructions between
// drawRestoreInterrupts and lumpFindResource. 0x20000000 is the observed
// OR immediate. Naming it the uncached segment is inferred from the public
// EE address map.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store32(unsigned address, unsigned value);
extern "C" void norrath_host_store16(unsigned address, unsigned value);
extern "C" void norrath_host_sync();
#define NORRATH_STORE32(address, value) norrath_host_store32((address), (value))
#define NORRATH_STORE16(address, value) norrath_host_store16((address), (value))
#define NORRATH_SYNC() norrath_host_sync()
#else
#define NORRATH_STORE32(address, value) \
    (*reinterpret_cast<volatile unsigned*>((address)) = (value))
#define NORRATH_STORE16(address, value) \
    (*reinterpret_cast<volatile unsigned short*>((address)) = static_cast<unsigned short>(value))
#define NORRATH_SYNC() asm volatile("sync" ::: "memory")
#endif

extern "C" {
int AddIntcHandler(int cause, void (*handler)(int), int next);
void vblankHandler__Fi(int value);
void EnableIntc(int cause);
int GetThreadId();
void ChangeThreadPriority(int thread, int priority);
void drawRestoreInterrupts__Fv();
unsigned char* lumpFindResource__FPcT0(char* lump, char* name);
void sceDmaSendN(unsigned channel, unsigned address, unsigned count);
void initSupersample__Fv();
void initVIF__Fv();
void padInit__Fv();

extern int vblankHID;
extern int engineShadowTexAddress;
extern unsigned dmaGif;

char norrath_engine_restore_engine_lump[] = "engine.lmp";
char norrath_engine_restore_shadowblob[] = "shadowblob.tex";

void engineRestore__Fv() {
    vblankHID = AddIntcHandler(2, vblankHandler__Fi, 0);
    EnableIntc(2);
    ChangeThreadPriority(GetThreadId(), 10);
    drawRestoreInterrupts__Fv();
    NORRATH_STORE32(0x10000010u, 130u);
    NORRATH_STORE32(0x10000000u, 0u);
    NORRATH_STORE32(0x10000810u, 159u);
    unsigned char* resource = lumpFindResource__FPcT0(
        norrath_engine_restore_engine_lump, norrath_engine_restore_shadowblob);
    unsigned mapped = *reinterpret_cast<unsigned*>(resource + 16);
    // sra by 8, then sh. The filled bits sit above bit 15, so the stored
    // halfword is bits 8..23. A negative word's C++ shift is the observed
    // opcode; this positive-bit pattern does not depend on that choice.
    unsigned shifted = static_cast<unsigned>(engineShadowTexAddress >> 8) & 0xffffu;
    NORRATH_STORE16((mapped | 0x20000000u) + 20u, shifted);
    NORRATH_SYNC();
    unsigned count = *reinterpret_cast<unsigned short*>(resource + 6);
    sceDmaSendN(dmaGif, mapped, count);
    initSupersample__Fv();
    initVIF__Fv();
    padInit__Fv();
}
}
