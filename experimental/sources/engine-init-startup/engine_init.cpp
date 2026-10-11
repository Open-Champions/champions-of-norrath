// Observed engineInit__Ff at 0x001902b0, 1944 bytes. main calls it with the
// .lit4 word 0x3f7fffff. This body is the call and store sequence from that
// extent. Fresh compilation is not an original-byte match. No EE path has run.
//
// 0x01fc0000 is the address of _stack. 0x008ec91c is the address of _end.
// The 32-bit memory test compares 0x01ffffff with that stack base. The taken
// arm uses 0x02000000. The other arm is in the image: stack top 0x08000000
// and heap top 0x05a00000. Naming the GS and timer addresses below is inferred
// from the public EE register map; the source uses the addresses.
//
// GS: 0x12000000, 0x12000020, 0x12000070, 0x12000080, 0x12000090, 0x120000a0.
// DMA priority word: 0x1000e020. Timer words: 0x10000000, 0x10000010, 0x10000810.
//
// The negative halfword arms reload displayBuffer1StartAddress and run only
// when that word, or the second sum, is negative. This image stores a positive
// word. Whether a callee changes it before the reload is unknown.
// sprintf, sscanf, and strtod run; this function does not read their results.
// vblankHandler__Fi, floorInit__Fv, and the other calls stay imports.
#include "ee_mmio.hpp"

extern "C" {
extern volatile int vblankCount;
extern volatile unsigned char vblankField;
extern int eRandState;
extern int vblankSemaphore;
extern int vblankSemaphore2;
extern int vblankHID;
extern int displayBuffer1StartAddress;
extern unsigned displayBuffer2StartAddress;
extern unsigned engineEnvTexAddress;
extern int engineShadowTexAddress;
extern unsigned textureBufferStartAddress;
extern unsigned engineDrawBufferStartAddress;
extern int engineDisplayDisable;
extern int memAllocSize;
extern void* memAllocPtr;
extern unsigned* dmaGif;
extern unsigned* dmaVif0;
extern unsigned* dmaVif1;
extern unsigned char dispEnv[];
extern unsigned char ntsc_legallineTex[];
extern unsigned char ntsc_legallinebottomTex[];
extern char* defaultFont;
extern char* haloTexture1;
extern char engineCurrentLevelName[];

int GetThreadId();
void ChangeThreadPriority(int thread, int priority);
int CreateSema(unsigned* param);
void sceGsResetGraph(int mode, int interlace, int ffmode, int version);
void vblankHandler__Fi(int cause);
int AddIntcHandler(int cause, void (*handler)(int), int next);
void EnableIntc(int cause);
void WaitSema(int id);
unsigned* sceDmaGetChan(int channel);
void textureInitTexture__FP8_texture(unsigned char* texture);
void sceDmaSendN(unsigned* channel, void* packet, int qwc);
void sceGsSyncPath(int mode, int timeout);
void sceGsSetDefDispEnv(unsigned char* env, int a, int b, int c, int d, int e);
void sceGsPutDispEnv(unsigned char* env);
void* __wrap_malloc(unsigned size);
#if defined(NORRATH_HOST_CONTRACT)
int norrath_host_sprintf(char* destination, const char* format, ...);
int norrath_host_sscanf(const char* text, const char* format, ...);
double norrath_host_strtod(const char* text, char** end);
void* norrath_host_memset(void* destination, int value, unsigned count);
#define NORRATH_SPRINTF norrath_host_sprintf
#define NORRATH_SSCANF norrath_host_sscanf
#define NORRATH_STRTOD norrath_host_strtod
#define NORRATH_MEMSET norrath_host_memset
#else
int sprintf(char* destination, const char* format, ...);
int sscanf(const char* text, const char* format, ...);
double strtod(const char* text, char** end);
void* memset(void* destination, int value, unsigned count);
#define NORRATH_SPRINTF sprintf
#define NORRATH_SSCANF sscanf
#define NORRATH_STRTOD strtod
#define NORRATH_MEMSET memset
#endif
void memInit__FPvi(void* block, int size);
void _engineCheckStack__FPCci(const char* site, int line);
void FlushCache(int mode);
void initSif__Fv();
void padInit__Fv();
int sceSifQueryMaxFreeMemSize();
int sceSifQueryTotalFreeMemSize();
void scePrintf(const char* text, ...);
void cdInit__Fv();
void sceGsResetPath();
void sceDmaReset(int mode);
void initVIF__Fv();
void emathInit__Fv();
void textureAllocateCacheBuffer__Fv();
void textureInit__Fii(int address, int size);
void drawInit__Fv();
void blockAllocInit__Fv();
void listInit__Fv();
void soundInit__Ff(float value);
void scriptInit__Fv();
void animInit__Fv();
void objectInit__Fv();
void worldInit__Fv();
void floorInit__Fv();
void initSupersample__Fv();
void lumpLoad__FPc(char* name);
char* lumpFindResource__FPcT0(char* lump, char* name);
void lumpLock__Fi(int value);
void P_Init__Fv();
void lightInit__Fv();
void sceDmaSync(unsigned* channel, int mode, int timeout);
}

static inline __attribute__((always_inline)) unsigned norrath_texture_word(const unsigned char* texture) {
    return static_cast<unsigned>(texture[16]) |
           (static_cast<unsigned>(texture[17]) << 8) |
           (static_cast<unsigned>(texture[18]) << 16) |
           (static_cast<unsigned>(texture[19]) << 24);
}

static inline __attribute__((always_inline)) unsigned short norrath_texture_qwc(const unsigned char* texture) {
    return static_cast<unsigned short>(texture[6] | (texture[7] << 8));
}

extern "C" void engineInit__Ff(float argument) {
    const float saved = argument;
    NORRATH_STORE64(0x12000000u, 0ull);
    eRandState = static_cast<int>(NORRATH_COP0_COUNT());

    const unsigned stack_base = NORRATH_STACK_BASE;
    const unsigned memory_last = NORRATH_MEMORY_LAST;
    const bool low_memory = memory_last >= stack_base;
    const unsigned stack_top = low_memory ? 0x02000000u : 0x08000000u;
    const unsigned stack_bytes = stack_top - stack_base;
    scePrintf("stack size=%d\n", static_cast<int>(stack_bytes));
    if (stack_bytes >= 5120u) {
        unsigned cursor = 5120u;
        do {
            NORRATH_STORE8(stack_top - cursor, 0xa5u);
            cursor += 1u;
        } while (stack_bytes >= cursor);
    }

    const int thread = GetThreadId();
    ChangeThreadPriority(thread, 10);
    unsigned first_param[3];
    first_param[1] = 0x7ffffc17u;
    first_param[2] = 0u;
    vblankSemaphore = CreateSema(first_param);
    unsigned second_param[3];
    second_param[1] = 0x7ffffc17u;
    second_param[2] = 0u;
    const int second_sema = CreateSema(second_param);
    displayBuffer2StartAddress = 0x002bc000u;
    engineEnvTexAddress = 0x00348000u;
    engineShadowTexAddress = 0x00358000;
    displayBuffer1StartAddress = 0x00230000;
    textureBufferStartAddress = 0x00359000u;
    vblankSemaphore2 = second_sema;
    engineDrawBufferStartAddress = 0u;
    sceGsResetGraph(0, 1, 2, 1);

    vblankHID = AddIntcHandler(2, vblankHandler__Fi, 0);
    EnableIntc(2);
    {
        const int seen = vblankCount;
        if (vblankCount == seen) {
            do WaitSema(vblankSemaphore2);
            while (vblankCount == seen);
        }
        const volatile unsigned char field_now = vblankField;
        (void)field_now;
    }
    NORRATH_STORE64(0x12000000u, 0ull);
    {
        const int seen = vblankCount;
        if (vblankCount == seen) {
            do WaitSema(vblankSemaphore2);
            while (vblankCount == seen);
        }
        const volatile unsigned char field_now = vblankField;
        (void)field_now;
    }

    unsigned* channel = sceDmaGetChan(2);
    textureInitTexture__FP8_texture(ntsc_legallineTex);
    int adjusted = displayBuffer1StartAddress;
    if (adjusted < 0) adjusted += 255;
    unsigned short half = static_cast<unsigned short>(adjusted >> 8);
    unsigned packet = norrath_texture_word(ntsc_legallineTex);
    unsigned short qwc = norrath_texture_qwc(ntsc_legallineTex);
    NORRATH_STORE16((packet | 0x20000000u) + 20u, half);
    NORRATH_SYNC();
    sceDmaSendN(channel, reinterpret_cast<void*>(static_cast<unsigned long>(packet)), qwc);
    sceGsSyncPath(0, 0);

    channel = sceDmaGetChan(2);
    textureInitTexture__FP8_texture(ntsc_legallinebottomTex);
    const unsigned buffer_word = static_cast<unsigned>(displayBuffer1StartAddress);
    int sum = static_cast<int>(buffer_word + 0x0008c000u);
    const int sum_if_negative = static_cast<int>(buffer_word + 0x0008c0ffu);
    if (sum < 0) sum = sum_if_negative;
    half = static_cast<unsigned short>(sum >> 8);
    packet = norrath_texture_word(ntsc_legallinebottomTex);
    qwc = norrath_texture_qwc(ntsc_legallinebottomTex);
    NORRATH_STORE16((packet | 0x20000000u) + 20u, half);
    NORRATH_SYNC();
    sceDmaSendN(channel, reinterpret_cast<void*>(static_cast<unsigned long>(packet)), qwc);
    sceGsSyncPath(0, 0);

    NORRATH_STORE64(0x12000090u, 5120ull);
    sceGsSetDefDispEnv(dispEnv, 0, 640, 224, 0, 0);
    sceGsPutDispEnv(dispEnv);
    for (int pass = 0; pass < 3; ++pass) {
        const int seen = vblankCount;
        if (vblankCount == seen) {
            do WaitSema(vblankSemaphore2);
            while (vblankCount == seen);
        }
        const volatile unsigned char field_now = vblankField;
        (void)field_now;
    }

    NORRATH_STORE64(0x12000000u, 2ull);
    NORRATH_STORE64(0x12000020u, 1ull);
    const unsigned long long frame = static_cast<unsigned>(displayBuffer1StartAddress >> 13) | 0x1400u;
    const unsigned long long dispfb1 = frame | (0x8000ull << 28);
    unsigned long long display = 0x001be9ffull;
    display = (display << 16) | 0x183ull;
    display = (display << 16) | 0x227cull;
    NORRATH_STORE64(0x12000070u, dispfb1);
    NORRATH_STORE64(0x12000090u, frame);
    NORRATH_STORE64(0x12000080u, display);
    NORRATH_STORE64(0x120000a0u, display);

    char formatted[64];
    union {
        unsigned long long bits;
        double value;
    } literal;
    literal.bits = 0x3fb9999980000000ull;
    engineDisplayDisable = 0;
    NORRATH_SPRINTF(formatted, "a%d%f\n", 1, literal.value);
    double scanned = 0.0;
    NORRATH_SSCANF(".1", "%lf", &scanned);
    (void)scanned;
    NORRATH_STRTOD("1.0004", 0);

    const unsigned heap_top = low_memory ? 0x02000000u : 0x05a00000u;
    const unsigned heap_adjusted = (heap_top - NORRATH_LINK_END) + 0xfffb1000u;
    unsigned char* block = static_cast<unsigned char*>(__wrap_malloc(heap_adjusted - 4096u));
    const unsigned tail = heap_adjusted - 5120u;
    memAllocSize = static_cast<int>(tail);
    memAllocPtr = block;
    NORRATH_MEMSET(block + tail, 0xa5, 1024u);
    memInit__FPvi(block, static_cast<int>(tail));

    char site[] = "engine.cc";
    _engineCheckStack__FPCci(site, 1075);
    FlushCache(0);
    initSif__Fv();
    padInit__Fv();

    unsigned max_kb = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    unsigned total_kb = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf("preCD IOP %dk %dk\n", max_kb, total_kb);
    cdInit__Fv();
    max_kb = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total_kb = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf("postCD IOP %dk %dk\n", max_kb, total_kb);
    sceGsResetPath();
    sceDmaReset(1);

    unsigned* gif = sceDmaGetChan(2);
    const unsigned cleared = (*gif) & 0xffffffbfu;
    dmaGif = gif;
    *gif = cleared;
    unsigned* vif0 = sceDmaGetChan(0);
    const unsigned vif0_word = (*vif0) | 0x40u;
    dmaVif0 = vif0;
    *vif0 = vif0_word;
    FlushCache(0);
    unsigned* vif1 = sceDmaGetChan(1);
    const unsigned vif1_word = (*vif1) | 0x40u;
    dmaVif1 = vif1;
    *vif1 = vif1_word;
    FlushCache(0);

    NORRATH_STORE32(0x1000e020u, 256u);
    (void)NORRATH_LOAD32(0x1000e020u);
    initVIF__Fv();
    NORRATH_STORE32(0x10000010u, 130u);
    NORRATH_STORE32(0x10000000u, 0u);
    NORRATH_STORE32(0x10000810u, 159u);
    emathInit__Fv();
    textureAllocateCacheBuffer__Fv();
    textureInit__Fii(static_cast<int>(textureBufferStartAddress),
                     static_cast<int>(0x00400000u - textureBufferStartAddress));
    drawInit__Fv();
    blockAllocInit__Fv();
    listInit__Fv();

    max_kb = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total_kb = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf("preSound IOP %dk %dk\n", max_kb, total_kb);
    soundInit__Ff(saved);
    max_kb = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total_kb = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf("postSound IOP %dk %dk\n", max_kb, total_kb);
    scriptInit__Fv();
    animInit__Fv();
    objectInit__Fv();
    worldInit__Fv();
    floorInit__Fv();
    initSupersample__Fv();

    char engine_lump[] = "engine.lmp";
    char font_lump[] = "font.lmp";
    char gamefont[] = "gamefonta.fnt";
    char shadow_name[] = "shadowblob.tex";
    char halo_name[] = "halo1.tex";
    lumpLoad__FPc(engine_lump);
    lumpLoad__FPc(font_lump);
    defaultFont = lumpFindResource__FPcT0(font_lump, gamefont);
    lumpLock__Fi(100000);
    unsigned char* shadow = reinterpret_cast<unsigned char*>(lumpFindResource__FPcT0(engine_lump, shadow_name));
    half = static_cast<unsigned short>(engineShadowTexAddress >> 8);
    packet = norrath_texture_word(shadow);
    qwc = norrath_texture_qwc(shadow);
    NORRATH_STORE16((packet | 0x20000000u) + 20u, half);
    NORRATH_SYNC();
    sceDmaSendN(dmaGif, reinterpret_cast<void*>(static_cast<unsigned long>(packet)), qwc);
    haloTexture1 = lumpFindResource__FPcT0(engine_lump, halo_name);
    P_Init__Fv();
    lightInit__Fv();

    engineCurrentLevelName[0] = 0;
    {
        const int seen = vblankCount;
        if (vblankCount == seen) {
            do WaitSema(vblankSemaphore2);
            while (vblankCount == seen);
        }
        const volatile unsigned char field_now = vblankField;
        (void)field_now;
    }
    sceDmaSync(dmaGif, 0, 0);
    FlushCache(0);
    max_kb = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total_kb = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf("postInit IOP %dk %dk\n", max_kb, total_kb);
}
