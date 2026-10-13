// Host contract for __15AnimationState8RC15AnimationState8; not an EE input.
// Objects are byte buffers so offsets are the real ones; the pointer words in the object are
// 32-bit, so records are kept in a table of fake 32-bit "addresses" mapped by the host stub.
// Because the source converts those words with a pointer-width cast, the host run only exercises
// the zero-pointer and flag-mask paths and the plain block copy. The record fill is compared
// against the original in a private R5900 run.
#include "animation_state8_copy.cpp"
#include <cstdio>
#include <cstring>

static int freeCalls;
void* getFreeFrameState__Fv() { ++freeCalls; return nullptr; }
int main() {
    int bad = 0;
    alignas(8) static unsigned char src[264], dst[264];
    for (int i = 0; i < 264; ++i) src[i] = static_cast<unsigned char>(i * 7 + 3);
    src[5] = 3;
    for (int i = 0; i < 3; ++i) { unsigned zero = 0; std::memcpy(src + 12 + 32 * i, &zero, 4); }
    std::memset(dst, 0xcc, sizeof dst);
    __15AnimationState8RC15AnimationState8(dst, src);
    unsigned word0; std::memcpy(&word0, dst, 4);
    if (word0 != 0) { std::puts("fail: word +0 cleared"); bad = 1; }
    if (dst[5] != 3) { std::puts("fail: count byte copied (not the preset 1)"); bad = 1; }
    for (int i = 6; i < 264; ++i) {
        bool zeroed = false;
        for (int slot = 0; slot < 3; ++slot) if (i >= 12 + 32 * slot && i < 16 + 32 * slot) zeroed = true;
        if (!zeroed && dst[i] != src[i]) { std::printf("fail: byte %d not copied\n", i); bad = 1; break; }
    }
    if (freeCalls != 0) { std::puts("fail: no record may be taken for null source records"); bad = 1; }
    // Non-null source record with the flag bits clear: slot word cleared, no record taken.
    unsigned fake = 0x1000; std::memcpy(src + 12, &fake, 4);
    unsigned short flags = 0x0400; std::memcpy(src + 36, &flags, 2);
    __15AnimationState8RC15AnimationState8(dst, src);
    unsigned slot0; std::memcpy(&slot0, dst + 12, 4);
    if (slot0 != 0 || freeCalls != 0) { std::puts("fail: flags clear -> slot cleared, no record"); bad = 1; }
    // Negative count: no slot is visited.
    src[5] = 0xff; flags = 0x4800; std::memcpy(src + 36, &flags, 2);
    __15AnimationState8RC15AnimationState8(dst, src);
    std::memcpy(&slot0, dst + 12, 4);
    if (slot0 != 0x1000 || freeCalls != 0) { std::puts("fail: negative count visits no slot"); bad = 1; }
    return bad;
}
