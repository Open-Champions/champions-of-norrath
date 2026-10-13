#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
static void setp(unsigned char* field, const unsigned char* target) { uint32_t v = static_cast<uint32_t>(target - arena); memcpy(field, &v, 4); }
static void setw(unsigned char* p, int v) { memcpy(p, &v, 4); }
static int getw(const unsigned char* p) { int v; memcpy(&v, p, 4); return v; }
static float getf(const unsigned char* p) { float v; memcpy(&v, p, 4); return v; }
static unsigned short getw16(const unsigned char* p) { unsigned short v; memcpy(&v, p, 2); return v; }
static void setw16(unsigned char* p, int v) { unsigned short s = static_cast<unsigned short>(v); memcpy(p, &s, 2); }

static int surfaceCalls, findCalls, lockCalls, unlockCalls, lockResult, unlockResult, lockPitch;
static unsigned char* lockBuffer; static void* surfaceMemory;
static int findLog[16];
extern "C" void* Surface__8VIRasteri(void*, int) { ++surfaceCalls; return surfaceMemory; }
extern "C" int FindChar__7VIRFontUs(void*, unsigned short ch) { findLog[findCalls++] = ch; int t = ch - 'a'; if (t >= 0 && t < 6) return t; return ch == '?' ? 3 : -1; }
extern "C" int LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(void*, int level, int* pitch, int* store, unsigned char** buffer) {
    ++lockCalls; if (level != 0) return -50; *pitch = lockPitch; *store = 0; *buffer = lockBuffer; return lockResult;
}
extern "C" int UnlockMipLevel__9VISurfacei(void*, int level) { ++unlockCalls; return level == 0 ? unlockResult : -50; }
extern "C" void* VICopyBytes(void* destination, const void* source, unsigned int length) { return memcpy(destination, source, length); }
#include "cache.cpp"

int main() {
    alignas(16) static unsigned char font[256], surface[64];
    surfaceMemory = surface;
    unsigned char* glyphs = arena + 0x100;      // 12-byte records, 6 of them
    unsigned char* slots = arena + 0x300;       // 56-byte records, 4 of them
    unsigned char* data = arena + 0x600;
    unsigned char* lockMem = arena + 0x1000; lockBuffer = lockMem; lockPitch = 16;
    char text[] = "abc";
    int out[8];
    for (int variant = 0; variant < 4; ++variant) {
        memset(arena, 0, sizeof arena); memset(font, 0, sizeof font); memset(out, 0xee, sizeof out);
        setw(surface + 8, 128); setw(surface + 12, 64);
        setp(font + 64, glyphs); setp(font + 68, slots); setp(font + 84, data);
        setw(font + 20, 2); setw(font + 44, variant == 3 ? 2 : 8);
        // LRU list: slots 0 -> 1 -> 2 -> 3
        setw(font + 56, 0); setw(font + 60, 3);
        for (int k = 0; k < 4; ++k) {
            unsigned char* s = slots + 56 * k;
            setw(s + 4, k == 3 ? -1 : k + 1); setw(s + 8, k == 0 ? -1 : k - 1);
            setw(s, 64 * k); setw(s + 16, 10 * k); setw(s + 20, 100 + k); setw16(s + 12, 0xffff);
        }
        for (int g = 0; g < 6; ++g) {
            unsigned char* e = glyphs + 12 * g;
            setw(e, 4 * g); setw16(e + 4, 4); setw16(e + 8, 0xffff); e[10] = static_cast<unsigned char>(5 + g);
        }
        for (int i = 0; i < 64; ++i) data[i] = static_cast<unsigned char>(0x80 + i);
        // 'b' (glyph 1) is already cached in slot 2.
        if (variant >= 1) { setw16(glyphs + 12 + 8, 2); setw16(slots + 56 * 2 + 12, 1); }
        surfaceCalls = findCalls = lockCalls = unlockCalls = 0; lockResult = 0; unlockResult = variant == 2 ? -1 : 0;
        int ret = Cache__7VIRFontR8VIRasterPCciPi(font, nullptr, text, 8, out);
        if (surfaceCalls != 1) return 1;
        if (variant == 3) { if (ret != -1) return 2; continue; }   // capacity word below the count
        if (variant == 2) { if (ret != -1 || unlockCalls != 1) return 3; continue; }
        if (ret != 3 || out[0] != 0 || out[1] != 1 || out[2] != 2) return 4;
        if (findCalls != 3) return 5;
        int expectLocks = variant == 0 ? 1 : 1;   // 'a' and 'c' always miss
        if (lockCalls != expectLocks || unlockCalls != 1) return 6;
        // Glyph 0 took the head slot (0), glyph 2 the next head.
        if (getw16(glyphs + 8) != 0 || getw16(slots + 12) != 0) return 7;
        // Texture quad of glyph 0 in slot 0: x = 0, y = 100, H = 2, w = 5.
        const unsigned char* s0 = slots;
        float iw = 1.0f / 128.0f, ih = 1.0f / 64.0f;
        if (getf(s0 + 24) != 0.0f || getf(s0 + 28) != 100.0f * ih || getf(s0 + 32) != 5.0f * iw || getf(s0 + 36) != 100.0f * ih) return 8;
        if (getf(s0 + 40) != 5.0f * iw || getf(s0 + 44) != 102.0f * ih || getf(s0 + 48) != 0.0f || getf(s0 + 52) != 102.0f * ih) return 9;
        // Rows copied: glyph 0 source offset 0, 4 bytes per row, 2 rows, pitch 16, target slot offset 0.
        for (int row = 0; row < 2; ++row) for (int b = 0; b < 4; ++b) if (lockMem[row * 16 + b] != data[row * 4 + b]) return 10;
        // The LRU list now ends with the three used slots; every used slot appears exactly once.
        int seen = 0; int cur = getw(font + 56); int steps = 0;
        while (cur != -1 && steps < 8) { seen |= 1 << cur; cur = getw(slots + 56 * cur + 4); ++steps; }
        if (seen != 0xf || steps != 4) return 11;
        if (getw(font + 60) != 2 && getw(font + 60) != 1 && getw(font + 60) != 3 && getw(font + 60) != 0) return 12;
    }
    return 0;
}
