#include <cstdio>
#include <cstring>

extern "C" unsigned char drawRecords[0x10000];
extern "C" unsigned char drawHead[0x40];
extern "C" unsigned char endTagVU1[0x80];
extern "C" int writeBuffer;
extern "C" int nmDrawUsed;
extern "C" int endDrawSemaphore;
extern "C" int vif0HID;
extern "C" int vif1HID;
extern "C" int gifHID;
extern "C" void reloadDMA__Fi(int);
extern "C" void gsIntHandler__Fi(int);
extern "C" void drawInit__Fv();
extern "C" unsigned* norrath_draw_dma_gif();
extern "C" unsigned* norrath_draw_dma_vif0();
extern "C" unsigned* norrath_draw_dma_vif1();

struct Event {
    const char* name;
    unsigned long a;
    unsigned long b;
};
static Event events[32];
static int count;
static int sema_words[6];
static unsigned channels[3];

static void push(const char* name, unsigned long a, unsigned long b) {
    events[count].name = name;
    events[count].a = a;
    events[count].b = b;
    ++count;
}

extern "C" void norrath_store32(unsigned address, unsigned value) { push("w32", address, value); }
extern "C" void norrath_store64(unsigned address, unsigned long long value) {
    push("w64", address, static_cast<unsigned long>(value));
}
extern "C" unsigned* sceDmaGetChan(int channel) {
    push("chan", static_cast<unsigned>(channel), 0);
    return &channels[channel];
}
extern "C" void FlushCache(int mode) { push("flush", static_cast<unsigned>(mode), 0); }
extern "C" int CreateSema(int* words) {
    std::memcpy(sema_words, words, sizeof sema_words);
    push("sema", 0, 0);
    return 77;
}
extern "C" int AddDmacHandler(int channel, void (*handler)(int), int next) {
    push("dmac", static_cast<unsigned>(channel),
         reinterpret_cast<unsigned long>(handler) ^ static_cast<unsigned>(next));
    return 100 + channel;
}
extern "C" void EnableDmac(int channel) { push("edmac", static_cast<unsigned>(channel), 0); }
extern "C" int AddIntcHandler(int cause, void (*handler)(int), int next) {
    push("intc", static_cast<unsigned>(cause),
         reinterpret_cast<unsigned long>(handler) ^ static_cast<unsigned>(next));
    return 9;
}
extern "C" void EnableIntc(int cause) { push("eintc", static_cast<unsigned>(cause), 0); }
extern "C" void reloadDMA__Fi(int) {}
extern "C" void gsIntHandler__Fi(int) {}

static unsigned word_at(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

int main() {
    channels[0] = 0x1u;
    channels[1] = 0u;
    channels[2] = 0x40u;
    std::memset(drawRecords, 0x5a, sizeof drawRecords);
    std::memset(drawHead, 0x5a, sizeof drawHead);
    std::memset(endTagVU1, 0x5a, sizeof endTagVU1);
    count = 0;
    drawInit__Fv();
    if (word_at(endTagVU1) != 0x10000002u || word_at(endTagVU1 + 4) != 0u) return 1;
    if (word_at(endTagVU1 + 8) != 0x11000000u) return 2;
    if (word_at(endTagVU1 + 12) != 0x50000002u) return 3;
    if (word_at(endTagVU1 + 16) != 0x8001u) return 4;
    if (word_at(endTagVU1 + 20) != 0x10000000u) return 5;
    if (word_at(endTagVU1 + 24) != 14u) return 6;
    if (word_at(endTagVU1 + 28) != 0u || word_at(endTagVU1 + 32) != 0u) return 7;
    if (word_at(endTagVU1 + 36) != 2u || word_at(endTagVU1 + 40) != 97u) return 8;
    if (word_at(endTagVU1 + 44) != 0u) return 9;
    if (channels[0] != 0x41u || channels[1] != 0x40u || channels[2] != 0u) return 10;
    if (norrath_draw_dma_vif0() != &channels[0] || norrath_draw_dma_vif1() != &channels[1]) return 11;
    if (norrath_draw_dma_gif() != &channels[2]) return 12;
    if (writeBuffer != 1 || nmDrawUsed != 8) return 13;
    for (int index = 0; index < 8; ++index) {
        const unsigned char* record = drawRecords + ((2048 + index) << 4);
        const unsigned expect = static_cast<unsigned>(reinterpret_cast<unsigned long>(record));
        if (word_at(drawHead + 32 + index * 4) != expect) return 20 + index;
        if (word_at(record + 12) != 0u) return 30 + index;
        if (record[0] != 0x5a) return 40 + index;
    }
    if (sema_words[0] != 0 || sema_words[1] != 0x186a0 || sema_words[2] != 0) return 14;
    if (sema_words[3] != 0 || sema_words[4] != 0 || sema_words[5] != 0) return 15;
    if (endDrawSemaphore != 77 || vif0HID != 100 || vif1HID != 101 || gifHID != 102) return 16;
    const char* names[] = {"chan", "flush", "chan", "flush", "chan", "flush", "sema",
                           "dmac", "edmac", "dmac", "edmac", "dmac", "edmac", "intc", "eintc",
                           "w64", "w32"};
    if (count != 17) return 17;
    for (int index = 0; index < 17; ++index) {
        if (std::strcmp(events[index].name, names[index]) != 0) return 50 + index;
    }
    if (events[0].a != 0 || events[2].a != 1 || events[4].a != 2) return 18;
    if (events[7].a != 0 || events[9].a != 1 || events[11].a != 2) return 60;
    if (events[7].b != reinterpret_cast<unsigned long>(reloadDMA__Fi)) return 61;
    if (events[13].a != 0 || events[13].b != reinterpret_cast<unsigned long>(gsIntHandler__Fi)) return 62;
    if (events[15].a != 0x12001010u || events[15].b != 32000u) return 63;
    if (events[16].a != 0x10003010u || events[16].b != 4u) return 64;
    std::puts("drawInit contract passed");
    return 0;
}
