#define NORRATH_HOST_CONTRACT
#include <cstdint>

#include "engine_restore.cpp"

static int error;
static int step;
static int handler_cause;
static void (*handler_seen)(int);
static int handler_next;
static int hid_at_enable;
static int enable_cause;
static int thread_seen;
static int priority_seen;
static unsigned stores32[4];
static unsigned store32_values[4];
static int store32_count;
static unsigned store16_address;
static unsigned store16_value;
static int syncs;
static char* lump_left;
static char* lump_right;
static unsigned dma_channel;
static unsigned dma_address;
static unsigned dma_count;
static int inits;

static void fail(int code) {
    if (error == 0) error = code;
}

int vblankHID = 3;
int engineShadowTexAddress;
unsigned dmaGif;
static unsigned char resource_bytes[32];

extern "C" void norrath_host_store32(unsigned address, unsigned value) {
    if (store32_count < 4) {
        stores32[store32_count] = address;
        store32_values[store32_count] = value;
    }
    ++store32_count;
}
extern "C" void norrath_host_store16(unsigned address, unsigned value) {
    store16_address = address;
    store16_value = value;
}
extern "C" void norrath_host_sync() { ++syncs; }

extern "C" int AddIntcHandler(int cause, void (*handler)(int), int next) {
    if (step != 0) fail(1);
    handler_cause = cause;
    handler_seen = handler;
    handler_next = next;
    ++step;
    return 5;
}
extern "C" void vblankHandler__Fi(int) {}
extern "C" void EnableIntc(int cause) {
    if (step != 1) fail(2);
    hid_at_enable = vblankHID;
    enable_cause = cause;
    ++step;
}
extern "C" int GetThreadId() {
    if (step != 2) fail(3);
    ++step;
    return 8;
}
extern "C" void ChangeThreadPriority(int thread, int priority) {
    if (step != 3) fail(4);
    thread_seen = thread;
    priority_seen = priority;
    ++step;
}
extern "C" void drawRestoreInterrupts__Fv() {
    if (step != 4 || store32_count != 0) fail(5);
    ++step;
}
extern "C" unsigned char* lumpFindResource__FPcT0(char* lump, char* name) {
    if (step != 5 || store32_count != 3) fail(6);
    lump_left = lump;
    lump_right = name;
    ++step;
    return resource_bytes;
}
extern "C" void sceDmaSendN(unsigned channel, unsigned address, unsigned count) {
    if (step != 6 || syncs != 1) fail(7);
    dma_channel = channel;
    dma_address = address;
    dma_count = count;
    ++step;
}
extern "C" void initSupersample__Fv() {
    if (step != 7) fail(8);
    ++step;
    ++inits;
}
extern "C" void initVIF__Fv() {
    if (step != 8) fail(9);
    ++step;
    ++inits;
}
extern "C" void padInit__Fv() {
    if (step != 9) fail(10);
    ++step;
    ++inits;
}

static void prepare(int shadow, unsigned mapped, unsigned short count) {
    error = 0;
    step = 0;
    store32_count = 0;
    syncs = 0;
    inits = 0;
    vblankHID = 3;
    dmaGif = 0xabcdu;
    engineShadowTexAddress = shadow;
    for (int i = 0; i < 32; ++i) resource_bytes[i] = 0;
    resource_bytes[6] = static_cast<unsigned char>(count);
    resource_bytes[7] = static_cast<unsigned char>(count >> 8);
    resource_bytes[16] = static_cast<unsigned char>(mapped);
    resource_bytes[17] = static_cast<unsigned char>(mapped >> 8);
    resource_bytes[18] = static_cast<unsigned char>(mapped >> 16);
    resource_bytes[19] = static_cast<unsigned char>(mapped >> 24);
}

int main() {
    prepare(0x00fedcba, 0x1000u, 0xffffu);
    engineRestore__Fv();
    if (error != 0) return error;
    if (step != 10 || inits != 3) return 11;
    if (handler_cause != 2 || handler_seen != vblankHandler__Fi || handler_next != 0) return 12;
    if (hid_at_enable != 5 || enable_cause != 2 || vblankHID != 5) return 13;
    if (thread_seen != 8 || priority_seen != 10) return 14;
    if (stores32[0] != 0x10000010u || store32_values[0] != 130u) return 15;
    if (stores32[1] != 0x10000000u || store32_values[1] != 0u) return 16;
    if (stores32[2] != 0x10000810u || store32_values[2] != 159u) return 17;
    if (lump_left != norrath_engine_restore_engine_lump || lump_right != norrath_engine_restore_shadowblob) return 18;
    if (store16_address != ((0x1000u | 0x20000000u) + 20u) || store16_value != 0xfedcu) return 19;
    if (dma_channel != 0xabcdu || dma_address != 0x1000u || dma_count != 0xffffu) return 20;

    prepare(0x00012300, 0u, 1u);
    engineRestore__Fv();
    if (error != 0) return error;
    if (store16_value != 0x0123u || dma_address != 0u || dma_count != 1u) return 21;
    return 0;
}
