#define NORRATH_HOST_CONTRACT
#include "do_super_sample.cpp"

#include <cassert>
#include <cstdio>
#include <cstring>

extern "C" int trippleBufferMode = 0;
extern "C" int compositeAlpha = 0;
extern "C" int compositeR = 0;
extern "C" int compositeG = 0;
extern "C" int compositeB = 0;
extern "C" float engineBrightnessCorrection = 0.0f;
extern "C" int displayBuffer1StartAddress = 0;
extern "C" int displayBuffer2StartAddress = 0;
extern "C" U64 frameStatePacket[16] = {};
extern "C" U64 copyScreen[16] = {};
extern "C" unsigned char* copyScreenFrame = nullptr;
extern "C" unsigned char* copyScreenColor = nullptr;
extern "C" unsigned char* copyScreenAlpha = nullptr;
extern "C" void* dmaGif = nullptr;

static int log_calls[64];
static int log_count;
enum { CALL_SAMPLE = 1, CALL_SEND = 2, CALL_SYNC = 3, CALL_FLUSH = 4 };
static int sample_arg;
static int sample_count;
static void* send_args[64][2];
static int send_count;
static int sync_args[64][2];
static int sync_count;

extern "C" void makeSamplePacket__Fb(int full) {
    assert(log_count < 64);
    log_calls[log_count++] = CALL_SAMPLE;
    sample_arg = full;
    ++sample_count;
}
extern "C" void sceDmaSend(void* channel, void* packet) {
    assert(log_count < 64 && send_count < 64);
    send_args[send_count][0] = channel;
    send_args[send_count][1] = packet;
    ++send_count;
    log_calls[log_count++] = CALL_SEND;
}
extern "C" int sceGsSyncPath(int mode, int timeout) {
    assert(log_count < 64 && sync_count < 64);
    sync_args[sync_count][0] = mode;
    sync_args[sync_count][1] = timeout;
    ++sync_count;
    log_calls[log_count++] = CALL_SYNC;
    return 0;
}
extern "C" int FlushCache(int mode) {
    assert(log_count < 16);
    assert(mode == 0);
    log_calls[log_count++] = CALL_FLUSH;
    return 0;
}

static unsigned char alpha_byte;
static unsigned char color_bytes[4];
static unsigned char frame_bytes[4];

static void reset_state() {
    log_count = 0;
    sample_arg = -1;
    sample_count = 0;
    send_count = 0;
    sync_count = 0;
    trippleBufferMode = 0;
    compositeAlpha = 0;
    compositeR = 0;
    compositeG = 0;
    compositeB = 0;
    engineBrightnessCorrection = 0.0f;
    displayBuffer1StartAddress = 0x280000;
    displayBuffer2StartAddress = 0x300000;
    std::memset(frameStatePacket, 0xaa, sizeof(frameStatePacket));
    std::memset(copyScreen, 0xbb, sizeof(copyScreen));
    alpha_byte = 0;
    std::memset(color_bytes, 0, sizeof(color_bytes));
    std::memset(frame_bytes, 0, sizeof(frame_bytes));
    copyScreenAlpha = &alpha_byte;
    copyScreenColor = color_bytes;
    copyScreenFrame = frame_bytes;
    dmaGif = reinterpret_cast<void*>(0x1234);
}

int main() {
    // Mode caching: rebuild only on change, with (mode != 0) as the flag.
    reset_state();
    madePacket_769 = -1;
    engineBrightnessCorrection = 0.1f;
    doSuperSample__Fiib(0, 5, false);
    assert(sample_arg == 1 && madePacket_769 == 5);
    const int calls_after_first = log_count;
    doSuperSample__Fiib(0, 5, false);
    assert(sample_arg == 1 && log_count == calls_after_first + 3);  // no rebuild
    for (int i = 0; i < log_count; ++i)
        if (log_calls[i] == CALL_SAMPLE) assert(i < calls_after_first);
    doSuperSample__Fiib(0, 0, false);
    assert(sample_arg == 0 && madePacket_769 == 0);

    // Packet path: tripple nonzero, gray alpha below 128. Full word check.
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 1;
    compositeAlpha = 100;
    compositeR = 64;
    compositeG = 32;
    compositeB = 16;
    engineBrightnessCorrection = 0.1f;
    doSuperSample__Fiib(0, 0, true);
    assert(log_calls[0] == CALL_SEND && send_args[0][0] == dmaGif &&
           send_args[0][1] == frameStatePacket);
    assert(log_calls[1] == CALL_SYNC && sync_args[0][0] == 0 && sync_args[0][1] == 0);
    assert(frameStatePacket[0] == 0x70000005ULL);
    assert(frameStatePacket[1] == 0);
    assert(frameStatePacket[2] == 0x1000000000008004ULL);
    assert(frameStatePacket[3] == 14);
    assert(frameStatePacket[4] == 0x0a2800000a3000ULL);
    assert(frameStatePacket[5] == 80 && frameStatePacket[6] == 0);
    assert(frameStatePacket[7] == 81 && frameStatePacket[8] == 0xe000000280ULL);
    assert(frameStatePacket[9] == 82 && frameStatePacket[10] == 2);
    assert(frameStatePacket[11] == 83);
    // Tail: alpha byte, color pack at 0.1 scale, steppers, frame word.
    assert(alpha_byte == 100);
    assert(color_bytes[0] == 13 && color_bytes[1] == 6 && color_bytes[2] == 3 &&
           color_bytes[3] == 0);
    assert(compositeR == 68 && compositeG == 36 && compositeB == 20);  // +4 each
    assert(frame_bytes[0] == 0x40 && frame_bytes[1] == 0x01 && frame_bytes[2] == 0x0a &&
           frame_bytes[3] == 0x00);  // 0x280000>>13 | 0xa0000 = 0xa0140
    assert(log_calls[log_count - 3] == CALL_FLUSH);
    assert(log_calls[log_count - 2] == CALL_SEND &&
           send_args[send_count - 1][0] == dmaGif &&
           send_args[send_count - 1][1] == copyScreen);
    assert(log_calls[log_count - 1] == CALL_SYNC);

    // Buffer select: nonzero buffer swaps the buffer word halves.
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 1;
    compositeAlpha = 100;
    engineBrightnessCorrection = 0.1f;
    doSuperSample__Fiib(1, 0, true);
    assert(frameStatePacket[4] == 0x0a3000000a2800ULL);
    assert(frame_bytes[0] == 0x80 && frame_bytes[1] == 0x01 &&
           frame_bytes[2] == 0x0a && frame_bytes[3] == 0x00);  // 0x300000>>13|0xa0000

    // Skips: tripple zero, and alpha exactly 128 (gray default path).
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 0;
    compositeAlpha = 100;
    engineBrightnessCorrection = 0.1f;
    doSuperSample__Fiib(0, 0, true);
    assert(frameStatePacket[0] == 0xaaaaaaaaaaaaaaaaULL);  // untouched
    for (int i = 0; i < log_count; ++i) assert(log_calls[i] != CALL_SAMPLE || true);
    assert(log_count == 3);  // flush, send copyScreen, sync only
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 1;
    engineBrightnessCorrection = 0.1f;
    doSuperSample__Fiib(0, 0, false);  // gray 128 is not below 128
    assert(frameStatePacket[0] == 0xaaaaaaaaaaaaaaaaULL);
    assert(alpha_byte == 128);

    // Brightness at/above 0.2 bumps compositeAlpha by 3 and pins scale 0.2.
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 0;
    compositeAlpha = 100;
    compositeR = 64;
    compositeG = 32;
    compositeB = 16;
    engineBrightnessCorrection = 0.5f;
    doSuperSample__Fiib(0, 0, true);
    assert(compositeAlpha == 103);
    assert(alpha_byte == 100);  // byte uses the pre-bump value
    assert(color_bytes[0] == 26 && color_bytes[1] == 13 && color_bytes[2] == 6);
    // NaN scale also takes the bump path.
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 0;
    compositeAlpha = 100;
    compositeR = 64;
    compositeG = 32;
    compositeB = 16;
    engineBrightnessCorrection = __builtin_nanf("");
    doSuperSample__Fiib(0, 0, true);
    assert(compositeAlpha == 103);
    assert(color_bytes[0] == 26 && color_bytes[1] == 13 && color_bytes[2] == 6);

    // Alpha clamps and color saturation (saturation needs out-of-band words,
    // since byte inputs stay below 255 at every reachable scale).
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 0;
    compositeAlpha = 200;
    compositeR = 250;
    compositeG = 250;
    compositeB = 250;
    engineBrightnessCorrection = 0.1f;
    doSuperSample__Fiib(0, 0, true);
    assert(alpha_byte == 128);
    assert(color_bytes[0] == 50 && color_bytes[1] == 50 && color_bytes[2] == 50);
    compositeR = 2000;
    compositeG = 2000;
    compositeB = 2000;
    doSuperSample__Fiib(0, 0, true);
    assert(color_bytes[0] == 255 && color_bytes[1] == 255 && color_bytes[2] == 255);
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 0;
    compositeAlpha = -5;
    compositeR = 250;
    compositeG = 250;
    compositeB = 250;
    engineBrightnessCorrection = 1.0f;  // >= 0.2, bump path, scale 0.2
    doSuperSample__Fiib(0, 0, true);
    assert(alpha_byte == 0);
    assert(color_bytes[0] == 100 && color_bytes[1] == 100 && color_bytes[2] == 100);

    // Stepper band edges on all three channels.
    reset_state();
    madePacket_769 = 0;
    trippleBufferMode = 0;
    engineBrightnessCorrection = 0.1f;
    compositeAlpha = 0;
    compositeR = 133;
    compositeG = 132;
    compositeB = 124;
    doSuperSample__Fiib(0, 0, true);
    assert(compositeR == 129 && compositeG == 128 && compositeB == 128);
    compositeR = 123;
    compositeG = 200;
    compositeB = 1;
    doSuperSample__Fiib(0, 0, true);
    assert(compositeR == 127 && compositeG == 196 && compositeB == 5);

    std::puts("do super sample contract passed");
    return 0;
}
