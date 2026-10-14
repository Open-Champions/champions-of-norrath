#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" {
alignas(16) unsigned char packetBuffer[256];
alignas(16) unsigned char dispEnv[48];
unsigned engineEnvTexAddress;
unsigned engineDrawBufferStartAddress;
unsigned long long zwritesOn;
unsigned long long zwritesOff;
unsigned* dmaGif;
int fullFrameMode;
void initSupersample__Fv();
}

static int calls;
static unsigned* seen_channel;
static void* seen_packet;
static int seen_flush;
static int seen_path_mode;
static int seen_path_timeout;
static int seen_sample;

extern "C" void FlushCache(int mode) { seen_flush = mode; ++calls; }
extern "C" void sceDmaSend(unsigned* channel, void* packet) {
    seen_channel = channel;
    seen_packet = packet;
    ++calls;
}
extern "C" void sceGsSyncPath(int mode, int timeout) {
    seen_path_mode = mode;
    seen_path_timeout = timeout;
    ++calls;
}
extern "C" void makeSamplePacket__Fb(int full_frame) { seen_sample = full_frame; ++calls; }

static unsigned long long qword_at(const unsigned char* address) {
    unsigned long long value = 0;
    for (int index = 0; index < 8; ++index)
        value |= static_cast<unsigned long long>(address[index]) << (8 * index);
    return value;
}

static unsigned word_at(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

static int expect_common() {
    if (qword_at(packetBuffer + 0x00) != 0x6000000bull) return 1;
    if (qword_at(packetBuffer + 0x08) != 0ull) return 2;
    if (qword_at(packetBuffer + 0x10) != 0x100000000000800aull) return 3;
    if (qword_at(packetBuffer + 0x18) != 14ull) return 4;
    if (qword_at(packetBuffer + 0x28) != 77ull || qword_at(packetBuffer + 0x38) != 79ull) return 5;
    if (qword_at(packetBuffer + 0x40) != 0x01bf000004ff0000ull) return 6;
    if (qword_at(packetBuffer + 0x50) != 0x01bf000004ff0000ull) return 7;
    if (qword_at(packetBuffer + 0x48) != 64ull || qword_at(packetBuffer + 0x58) != 65ull) return 8;
    if (qword_at(packetBuffer + 0x60) != 0x0000720000005800ull) return 9;
    if (qword_at(packetBuffer + 0x68) != 25ull || qword_at(packetBuffer + 0x70) != 42ull) return 10;
    if (qword_at(packetBuffer + 0x78) != 67ull || qword_at(packetBuffer + 0x80) != 0x50013ull) return 11;
    if (qword_at(packetBuffer + 0x88) != 72ull || qword_at(packetBuffer + 0x98) != 7ull) return 12;
    if (qword_at(packetBuffer + 0xa0) != 97ull || qword_at(packetBuffer + 0xa8) != 21ull) return 13;
    if (qword_at(packetBuffer + 0xb0) != 0ull || qword_at(packetBuffer + 0xb8) != 9ull) return 14;
    if (qword_at(packetBuffer + 192) != 0xa5a5a5a5a5a5a5a5ull) return 15;
    return 0;
}

int main() {
    unsigned channel = 0x44u;
    dmaGif = &channel;
    fullFrameMode = 5;
    engineEnvTexAddress = 0x00348000u;
    engineDrawBufferStartAddress = 0u;
    std::memset(dispEnv, 0, sizeof dispEnv);
    dispEnv[16] = 0xff;
    dispEnv[17] = 0x03;
    std::memset(packetBuffer, 0xa5, sizeof packetBuffer);
    calls = 0;
    initSupersample__Fv();
    if (int bad = expect_common()) return bad;
    if (qword_at(packetBuffer + 0x20) != 0x02140000ull) return 20;
    if (qword_at(packetBuffer + 0x30) != 0x0200008cull) return 21;
    if (qword_at(packetBuffer + 0x90) != 0x20000001dc00b480ull) return 22;
    if (zwritesOn != 0x0200008cull || zwritesOff != 0x010200008cull) return 23;
    if (word_at(dispEnv + 16) != 0x200u) return 24;
    if (calls != 4 || seen_flush != 0) return 25;
    if (seen_channel != &channel || seen_packet != packetBuffer) return 26;
    if (seen_path_mode != 0 || seen_path_timeout != 0 || seen_sample != 5) return 27;

    engineEnvTexAddress = 0x00348123u;
    engineDrawBufferStartAddress = 0x2000u;
    dispEnv[16] = 0xff;
    dispEnv[17] = 0xff;
    dispEnv[18] = 0x01;
    initSupersample__Fv();
    if (qword_at(packetBuffer + 0x20) != 0x02140001ull) return 30;
    if (qword_at(packetBuffer + 0x30) != 0x0200008dull) return 31;
    if (qword_at(packetBuffer + 0x90) != 0x20000001dc00b481ull) return 32;
    if (zwritesOn != 0x0200008dull || zwritesOff != 0x010200008dull) return 33;
    if (word_at(dispEnv + 16) != 0x1fe00u) return 34;

    engineEnvTexAddress = 0x100u;
    engineDrawBufferStartAddress = 0x1fffu;
    dispEnv[16] = 0x55;
    dispEnv[17] = 0x01;
    dispEnv[18] = 0;
    initSupersample__Fv();
    if (qword_at(packetBuffer + 0x90) != 0x20000001dc008001ull) return 40;
    if (qword_at(packetBuffer + 0x20) != 0x02140000ull) return 41;
    if (word_at(dispEnv + 16) != 0u) return 42;

    engineEnvTexAddress = 0xffffff00u;
    engineDrawBufferStartAddress = 0xffffe000u;
    initSupersample__Fv();
    if (qword_at(packetBuffer + 0x90) != 0xffffffffffffffffull) return 50;
    if (qword_at(packetBuffer + 0x20) != 0xffffffffffffffffull) return 51;
    if (qword_at(packetBuffer + 0x30) != 0x0200008bull) return 52;
    if (zwritesOn != 0x0200008bull || zwritesOff != 0x010200008bull) return 53;

    std::puts("initSupersample contract passed");
    return 0;
}
