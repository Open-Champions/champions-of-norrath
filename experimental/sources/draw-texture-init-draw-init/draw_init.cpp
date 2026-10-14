// Observed drawInit__Fv at 0x0015f6d0, 596 bytes. It fills the 48 bytes at
// endTagVU1 (0x00614240) and, when that buffer plus 0x30 is not 16-byte
// aligned, zeros bytes up to the next alignment. The 64-bit word at the
// start is 0x10000002 when no padding bytes are added. sceDmaGetChan(0)
// and sceDmaGetChan(1) have bit 0x40 set on their first word; channel 2
// has that bit cleared. Those pointers are the local words at 0x004868d8,
// 0x004868dc, and 0x004868d4, not the words at 0x00486a40. Eight record
// addresses, (2048+index)<<4 bytes into drawRecords (0x00604200), are
// stored at drawHead+32 (0x00614220) and each record's word at offset 12
// is cleared. writeBuffer becomes 1 and nmDrawUsed becomes 8. CreateSema
// receives 24 zero bytes with word 1 set to 0x186a0. reloadDMA__Fi is
// registered on DMAC channels 0, 1, and 2. gsIntHandler__Fi is registered
// on interrupt cause 0. 32000 is stored at 0x12001010 and 4 at 0x10003010.
// Fresh compilation is not an original-byte match. No EE path has run.

extern "C" {
alignas(16) unsigned char drawRecords[0x10000];
alignas(16) unsigned char drawHead[0x40];
alignas(16) unsigned char endTagVU1[0x80];
int writeBuffer;
int nmDrawUsed;
int endDrawSemaphore;
int vif0HID;
int vif1HID;
int gifHID;
void reloadDMA__Fi(int cause);
void gsIntHandler__Fi(int cause);
unsigned* sceDmaGetChan(int channel);
void FlushCache(int mode);
void* memset(void* destination, int value, unsigned count);
int CreateSema(int* words);
int AddDmacHandler(int channel, void (*handler)(int), int next);
void EnableDmac(int channel);
int AddIntcHandler(int cause, void (*handler)(int), int next);
void EnableIntc(int cause);
}

static unsigned* drawDmaGif;
static unsigned* drawDmaVif0;
static unsigned* drawDmaVif1;

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_store32(unsigned address, unsigned value);
extern "C" void norrath_store64(unsigned address, unsigned long long value);
extern "C" unsigned* norrath_draw_dma_gif() { return drawDmaGif; }
extern "C" unsigned* norrath_draw_dma_vif0() { return drawDmaVif0; }
extern "C" unsigned* norrath_draw_dma_vif1() { return drawDmaVif1; }
#else
static inline void norrath_store32(unsigned address, unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address)) = value;
}
static inline void norrath_store64(unsigned address, unsigned long long value) {
    *reinterpret_cast<volatile unsigned long long*>(static_cast<unsigned long>(address)) = value;
}
#endif

static void store_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" void drawInit__Fv() {
    unsigned char* const origin = endTagVU1;
    unsigned char* cursor = origin + 8;
    unsigned char* tail = cursor + 40;
    store_word(cursor + 0, 0x11000000u);
    store_word(cursor + 4, 0x50000002u);
    store_word(cursor + 8, 0x8001u);
    store_word(cursor + 12, 0x10000000u);
    store_word(cursor + 16, 14u);
    store_word(cursor + 28, 2u);
    store_word(cursor + 32, 97u);
    store_word(cursor + 20, 0u);
    store_word(cursor + 24, 0u);
    store_word(cursor + 36, 0u);
    if ((reinterpret_cast<unsigned long>(tail) & 0xful) != 0ul) {
        *tail = 0;
        while (true) {
            tail += 1;
            if ((reinterpret_cast<unsigned long>(tail) & 0xful) == 0ul) break;
            *tail = 0;
        }
    }
    unsigned span = static_cast<unsigned>(tail - origin);
    int qword_count = static_cast<int>(span >> 3);
    qword_count -= 2;
    qword_count >>= 1;
    const unsigned tag = static_cast<unsigned>(qword_count) | 0x10000000u;
    origin[0] = static_cast<unsigned char>(tag);
    origin[1] = static_cast<unsigned char>(tag >> 8);
    origin[2] = static_cast<unsigned char>(tag >> 16);
    origin[3] = static_cast<unsigned char>(tag >> 24);
    origin[4] = 0;
    origin[5] = 0;
    origin[6] = 0;
    origin[7] = 0;

    unsigned* channel = sceDmaGetChan(0);
    drawDmaVif0 = channel;
    *channel |= 0x40u;
    FlushCache(0);
    channel = sceDmaGetChan(1);
    drawDmaVif1 = channel;
    *channel |= 0x40u;
    FlushCache(0);
    channel = sceDmaGetChan(2);
    drawDmaGif = channel;
    *channel &= 0xffffffbfu;
    FlushCache(0);

    writeBuffer = 1;
    nmDrawUsed = 0;
    for (int index = 0; index < 8; ++index) {
        unsigned char* record = drawRecords + ((2048 + index) << 4);
        store_word(drawHead + 32 + index * 4,
                   static_cast<unsigned>(reinterpret_cast<unsigned long>(record)));
        store_word(record + 12, 0u);
    }
    nmDrawUsed = 8;

    int sema_words[6];
    memset(sema_words, 0, 24u);
    sema_words[1] = 0x186a0;
    sema_words[2] = 0;
    endDrawSemaphore = CreateSema(sema_words);
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
