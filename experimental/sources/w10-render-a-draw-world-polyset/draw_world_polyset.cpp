#include "../level-anim-load-common/mem.hpp"
// Observed drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi at 0x00179b40, 2764 bytes.
// Arguments: world header, polyset header and an integer (called "layer" here). Records are
// addressed by byte offset. Polyset fields used: +0 and +4 words, +32 word (offset of the
// texture inside its page), +36 flag word, +40, +44, +48 words, +52 word (low half is the
// texture id; bits 0x10000, 0x20000 and bit 18 select matrix rows and signs), +54 and +56
// signed halfwords. Order of work:
//  1. Flag 0x80 hands the call to drawWaterPatch, 0x800 to drawWorldObject (same arguments, and
//     the call ends). Flag 0x4000 calls drawWorldParticleEmitter(polyset) and ends unless
//     gameIsPaused__Fv() is nonzero; 0x8000 does the same with drawWorldDynamicLight. A nonzero
//     textureUncompBufferLocked byte, a zero texture id, a texturePageTable entry of -1 (which also
//     clears the byte worldLastDrawSucceded), or a texture whose word +20 is zero (texCacheData is
//     called first, and the byte is cleared) all end the call without drawing.
//  2. The texture is texturePageLoc[page] + polyset word +32. A packet (cursor, tag pointer,
//     saved pointer: three words on the stack) is started at glistp: the word 0x11000000 goes at
//     glistp+8 and the cursor starts at glistp+12, zero-padded until its low four bits are 12.
//  3. A direct-transfer word 0x50000005 (6 when flag 0x10 is set) then a quadword group at
//     cursor+4: (0x1000000000008000, 14), (0, 63), (0, 6) and either (42, 67) and (9, 66) when
//     flag 0x10 is set, or one quadword (72, 66) / (1, 66) / (2, 66) chosen by flags 0x10000 /
//     0x600 / neither; then (0x582f5 or 0x587f4 by flag 0x400, 71). The first group tag at the
//     saved pointer gets its low field OR-ed with the quadword count, and the words
//     0x04000000 | layer and 0x6c00c000-style word with ((flag 0x10) ? 6 : 4) << 16 follow.
//  4. Matrix data in floats: with flag 0x1 two rows of worldWorldViewMatrix chosen by bit 18 of
//     word +52 (rows swapped, and negated by 0x10000 / 0x20000); without it two combinations of
//     rows 0 and 1 weighted by the halfwords at +54 and +56 times 3.0518509e-05 (a second
//     combination is rotated, negated when flag 0x40 is clear). Rows 2 and 3 (eight floats)
//     follow. With flag 0x10 six camera-matrix floats times 6.0 follow (from worldCameraMatrix
//     rows chosen the same way when flag 0x1 is set). Then 192.0 and either 192.0 or
//     ((frameNm & 0x3ff) * 0.25 + 192.0) when levelFlags has bit 2, then the word 0x14000016.
//  5. Lighting: flag 0x100 is cleared and lightSetupAmbientWorld(&packet, -230) called; flag 0x2000
//     gives +230; flag 0x200 gives 0; otherwise lightSetupForWorld(&packet, polyset) decides; the
//     word 0x04000000 (0x04000001 when lightSetupForWorld returned zero) follows. Then the word
//     0x14000034, a quadword (0x30000000, +40, +44, +48) and zero padding to 16 bytes, a tag
//     quadword ((word0 << 32) | word4 | 0x30000000, 0) whose predecessor tag gets its count and
//     0x10000000, optionally (levelFlags bit 0) a (0x10000000, 0x1400003c) quadword and a second
//     copy of the tag, and finally the last tag's ID field is changed from 1 to 6 (or a
//     (0x60000000, 0) quadword is appended). glistp is advanced and drawAddToList is called with
//     the old list pointer, 4 when flag 0x200 is set and 0 otherwise, the texture and zeros.
// Inferred: this builds graphics-synthesizer and VU1 setup packets for a textured world polyset.
// Unknown: the exact meaning of the numeric tags and which polyset fields the flags correspond to.
extern "C" {
extern unsigned char textureUncompBufferLocked;
extern unsigned char worldLastDrawSucceded;
extern signed char texturePageTable[];
extern unsigned char* texturePageLoc[];
extern unsigned char* glistp;
extern int levelFlags;
extern int frameNm;
extern float worldWorldViewMatrix[16];
extern float worldCameraMatrix[12];
void drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(unsigned char* world, unsigned char* polyset, int layer);
void drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(unsigned char* world, unsigned char* polyset, int layer);
int gameIsPaused__Fv();
void drawWorldParticleEmitter__FP13PolysetHeader(unsigned char* polyset);
void drawWorldDynamicLight__FP13PolysetHeader(unsigned char* polyset);
void texCacheData__FP8_texturei(unsigned char* texture, int id);
void lightSetupAmbientWorld__FP6Packeti(unsigned char* packet, int amount);
int lightSetupForWorld__FP6PacketP13PolysetHeader(unsigned char* packet, unsigned char* polyset);
void drawAddToList__FPviP8_textureP11_drawRecordi(void* list, int kind, unsigned char* texture,
                                                  unsigned char* record, int extra);
}

namespace {
using namespace norrath_lal;
typedef long long i64;
typedef unsigned long long u64;

inline void store64(unsigned char* p, u64 value) {
    store_word(p, static_cast<unsigned>(value));
    store_word(p + 4, static_cast<unsigned>(value >> 32));
}
inline u64 load64(const unsigned char* p) {
    return static_cast<u64>(load_word(p)) | (static_cast<u64>(load_word(p + 4)) << 32);
}
inline void quad(unsigned char* p, u64 low, u64 high) {
    store64(p, low);
    store64(p + 8, high);
}
inline u64 signExtend(unsigned v) { return static_cast<u64>(static_cast<i64>(static_cast<int>(v))); }
inline unsigned low4(const unsigned char* p) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_address(p) & 15u;
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)) & 15u;
#endif
}
inline unsigned address(const unsigned char* p) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_address(p);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p));
#endif
}
inline void putFloat(unsigned char* p, float v) { store_float(p, v); }
inline void copyWord(unsigned char* to, const float* from) {
    store_word(to, *reinterpret_cast<const unsigned*>(from));
}
}  // namespace

extern "C" void drawWorldPolyset__FP12_worldHeaderP13PolysetHeaderi(unsigned char* world, unsigned char* polyset, int layer) {
    unsigned flags = load_word(polyset + 36);
    if ((flags & 0x80) != 0) {
        drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi(world, polyset, layer);
        return;
    }
    if ((flags & 0x800) != 0) {
        drawWorldObject__FP12_worldHeaderP13PolysetHeaderi(world, polyset, layer);
        return;
    }
    if ((flags & 0x4000) != 0) {
        if (gameIsPaused__Fv() == 0) {
            drawWorldParticleEmitter__FP13PolysetHeader(polyset);
            return;
        }
        flags = load_word(polyset + 36);
    }
    if ((flags & 0x8000) != 0) {
        if (gameIsPaused__Fv() == 0) {
            drawWorldDynamicLight__FP13PolysetHeader(polyset);
            return;
        }
    }
    if (static_cast<signed char>(textureUncompBufferLocked) != 0) return;
    const int id = load_signed_half(polyset + 52);
    if (id == 0) return;
    unsigned char* texture = nullptr;
    const int page = texturePageTable[id];
    if (page != -1) {
        texture = texturePageLoc[page] + load_word(polyset + 32);
        if (load_word(texture + 20) == 0) {
            texCacheData__FP8_texturei(texture, id);
            texture = nullptr;
            worldLastDrawSucceded = 0;
        }
    } else {
        worldLastDrawSucceded = 0;
    }
    if (texture == nullptr) return;
    if (load_word(texture + 20) == 0) return;

    unsigned char packet[12];
    unsigned char* list = glistp;
    store_word(list + 8, 0x11000000u);
    store_pointer(packet + 4, list);
    unsigned char* cursor = list + 12;
    store_pointer(packet + 0, cursor);
    while (low4(cursor) != 12) *cursor++ = 0;

    // Direct-transfer header and its register quadwords.
    {
        const unsigned f = load_word(polyset + 36);
        store_pointer(packet + 0, cursor);
        store_word(cursor, 0x50000000u + ((f & 0x10) != 0 ? 1u : 0u) + 5u);
        unsigned char* q = cursor + 4;
        quad(q, 0x1000000000008000ull, 14);
        const unsigned t2 = load_word(polyset + 36);
        store_pointer(packet + 8, q);
        quad(q + 16, 0, 63);
        store_pointer(packet + 0, q + 48);
        quad(q + 32, 0, 6);
        if ((t2 & 0x10) != 0) {
            store_pointer(packet + 0, q + 80);
            quad(q + 48, 42, 67);
            quad(q + 64, 9, 66);
        } else {
            u64 selector;
            if ((t2 & 0x10000) != 0) selector = 72;
            else if ((t2 & 0x600) != 0) selector = 1;
            else selector = 2;
            quad(q + 48, selector, 66);
            store_pointer(packet + 0, q + 64);
        }
        unsigned char* d = load_pointer(packet + 0);
        quad(d, (t2 & 0x400) != 0 ? 0x582f5 : 0x587f4, 71);
        store_pointer(packet + 0, d + 16);
    }
    unsigned char* t0 = load_pointer(packet + 0);
    {
        store_word(t0, 0x04000000u | static_cast<unsigned>(layer));
        unsigned char* saved = load_pointer(packet + 8);
        const unsigned f = load_word(polyset + 36);
        u64 tag = load64(saved);
        const int diff = static_cast<int>(address(t0) - address(saved));
        const i64 count = static_cast<i64>(((diff >> 3) - 2)) >> 1;
        tag |= static_cast<u64>(count);
        unsigned word = ((f >> 3) & 2u) + 4u;
        word <<= 16;
        word |= 0x60000000u | 0x0c00c000u;
        store64(saved, tag);
        store_pointer(packet + 0, t0 + 8);
        store_word(t0 + 4, word);
    }
    const unsigned t3 = load_word(polyset + 36);
    unsigned char* const t1 = t0 + 8;
    const unsigned char* const m = reinterpret_cast<const unsigned char*>(worldWorldViewMatrix);
    const unsigned char* const cam = reinterpret_cast<const unsigned char*>(worldCameraMatrix);
    if ((t3 & 1) != 0) {
        const unsigned w52 = load_word(polyset + 52);
        const unsigned row = (w52 >> 18) & 1u;
        const unsigned other = row ^ 1u;
        store_pointer(packet + 0, t0 + 24);
        const unsigned char* r = m + row * 16;
        if ((w52 & 0x10000) != 0) {
            putFloat(t1, -load_float(r + 0));
            putFloat(t0 + 12, -load_float(r + 4));
            putFloat(t0 + 16, -load_float(r + 8));
            putFloat(t0 + 20, -load_float(r + 12));
        } else {
            putFloat(t1, load_float(r + 0));
            putFloat(t0 + 12, load_float(r + 4));
            putFloat(t0 + 16, load_float(r + 8));
            putFloat(t0 + 20, load_float(r + 12));
        }
        const unsigned w52b = load_word(polyset + 52);
        const unsigned char* o = m + other * 16;
        unsigned char* d = load_pointer(packet + 0);
        if ((w52b & 0x20000) != 0) {
            putFloat(d, -load_float(o + 0));
            putFloat(d + 4, -load_float(o + 4));
            putFloat(d + 8, -load_float(o + 8));
            putFloat(d + 12, -load_float(o + 12));
        } else {
            putFloat(d, load_float(o + 0));
            putFloat(d + 4, load_float(o + 4));
            putFloat(d + 8, load_float(o + 8));
            putFloat(d + 12, load_float(o + 12));
        }
        store_pointer(packet + 0, d + 16);
    } else {
        float a = static_cast<float>(load_signed_half(polyset + 54));
        float b = static_cast<float>(load_signed_half(polyset + 56));
        const float scale = 0x1.0002p-15f;
        a = a * scale;
        b = b * scale;
        store_pointer(packet + 0, t0 + 24);
        const float m0 = load_float(m + 0), m1 = load_float(m + 4), m2 = load_float(m + 8), m3 = load_float(m + 12);
        const float m4 = load_float(m + 16), m5 = load_float(m + 20), m6 = load_float(m + 24), m7 = load_float(m + 28);
        putFloat(t1, a * m0 + b * m4);
        putFloat(t0 + 12, a * m1 + b * m5);
        putFloat(t0 + 16, a * m2 + b * m6);
        putFloat(t0 + 20, a * m3 + b * m7);
        store_pointer(packet + 0, t0 + 40);
        if ((t3 & 0x40) != 0) {
            putFloat(t0 + 24, b * m0 - a * m4);
            putFloat(t0 + 28, b * m1 - a * m5);
            putFloat(t0 + 32, b * m2 - a * m6);
            putFloat(t0 + 36, b * m3 - a * m7);
        } else {
            const float nb = -b;
            putFloat(t0 + 24, nb * m0 + a * m4);
            putFloat(t0 + 28, nb * m1 + a * m5);
            putFloat(t0 + 32, nb * m2 + a * m6);
            putFloat(t0 + 36, nb * m3 + a * m7);
        }
    }
    {
        unsigned char* d = load_pointer(packet + 0);
        for (int i = 0; i != 8; ++i) copyWord(d + i * 4, &worldWorldViewMatrix[8 + i]);
        store_pointer(packet + 0, d + 32);
    }
    if ((t3 & 0x10) != 0) {
        unsigned char* d = load_pointer(packet + 0);
        if ((t3 & 1) != 0) {
            const unsigned w52 = load_word(polyset + 52);
            const unsigned row = (w52 >> 18) & 1u;
            const unsigned other = row ^ 1u;
            const unsigned char* r = cam + row * 12;
            float x0, y0, x1, y1;
            if ((w52 & 0x10000) != 0) {
                y0 = -load_float(r + 4);
                x0 = -load_float(r + 0);
            } else {
                y0 = load_float(r + 4);
                x0 = load_float(r + 0);
            }
            const unsigned char* o = cam + other * 12;
            if ((w52 & 0x20000) != 0) {
                y1 = -load_float(o + 4);
                x1 = -load_float(o + 0);
            } else {
                y1 = load_float(o + 4);
                x1 = load_float(o + 0);
            }
            const float x2 = load_float(cam + 24);
            const float y2 = load_float(cam + 28);
            const float six = 6.0f;
            putFloat(d, x0 * six);
            putFloat(d + 4, y0 * six);
            putFloat(d + 8, x1 * six);
            putFloat(d + 12, y1 * six);
            putFloat(d + 16, x2 * six);
            putFloat(d + 20, y2 * six);
            store_pointer(packet + 0, d + 24);
        } else {
            const float six = 6.0f;
            putFloat(d, load_float(cam + 0) * six);
            putFloat(d + 4, load_float(cam + 4) * six);
            putFloat(d + 8, load_float(cam + 12) * six);
            putFloat(d + 12, load_float(cam + 16) * six);
            putFloat(d + 16, load_float(cam + 24) * six);
            putFloat(d + 20, load_float(cam + 28) * six);
            store_pointer(packet + 0, d + 24);
        }
        if ((levelFlags & 4) != 0) {
            unsigned char* e = load_pointer(packet + 0);
            const float base = static_cast<float>(frameNm & 0x3ff);
            putFloat(e, base * 0.25f + 192.0f);
            putFloat(e + 4, 192.0f);
            store_pointer(packet + 0, e + 8);
        } else {
            unsigned char* e = load_pointer(packet + 0);
            putFloat(e, 192.0f);
            putFloat(e + 4, 192.0f);
            store_pointer(packet + 0, e + 8);
        }
    }
    {
        unsigned char* d = load_pointer(packet + 0);
        store_word(d, 0x14000016u);
        store_pointer(packet + 0, d + 4);
    }
    unsigned mark = 0x04000000u;
    {
        const unsigned f = load_word(polyset + 36);
        if ((f & 0x100) != 0) {
            store_word(polyset + 36, f & ~0x100u);
            lightSetupAmbientWorld__FP6Packeti(packet, -230);
        } else if ((f & 0x2000) != 0) {
            lightSetupAmbientWorld__FP6Packeti(packet, 230);
        } else if ((f & 0x200) != 0) {
            lightSetupAmbientWorld__FP6Packeti(packet, 0);
        } else if (lightSetupForWorld__FP6PacketP13PolysetHeader(packet, polyset) == 0) {
            mark = 0x04000001u;
        }
        unsigned char* d = load_pointer(packet + 0);
        store_word(d, mark);
        store_pointer(packet + 0, d + 4);
    }
    {
        unsigned char* c = load_pointer(packet + 0);
        store_word(c, 0x14000034u);
        store_word(c + 4, 0x30000000u);
        unsigned char* pad = c + 24;
        store_pointer(packet + 0, pad);
        store_word(c + 8, load_word(polyset + 40));
        store_word(c + 12, load_word(polyset + 44));
        store_word(c + 20, 0);
        store_word(c + 16, load_word(polyset + 48));
        while (low4(pad) != 0) *pad++ = 0;
        store_pointer(packet + 0, pad);
    }
    {
        unsigned char* tagAt = load_pointer(packet + 4);
        unsigned char* const here = load_pointer(packet + 0);
        const int diff = static_cast<int>(address(here) - address(tagAt));
        const int count = ((diff >> 3) - 2) >> 1;
        const u64 body = (signExtend(load_word(polyset + 0)) << 32) | signExtend(load_word(polyset + 4));
        const u64 header = body | 0x30000000ull;
        store64(tagAt, signExtend(static_cast<unsigned>(count)) | 0x10000000ull);
        quad(here, header, 0);
        store_pointer(packet + 4, here);
        store_pointer(packet + 0, here + 16);
        if ((levelFlags & 1) != 0) {
            unsigned char* t1b = here + 16;
            quad(t1b, 0x10000000ull, 0x1400003cull);
            store_pointer(packet + 4, here + 32);
            quad(here + 32, header, 0);
            store_pointer(packet + 0, here + 48);
        }
    }
    {
        unsigned char* tag = load_pointer(packet + 4);
        const u64 current = load64(tag);
        if ((static_cast<i64>(current) >> 28 & 7) == 1) {
            store64(tag, (current & 0xFFFFFFFF8FFFFFFFull) | 0x60000000ull);
        } else {
            unsigned char* d = load_pointer(packet + 0);
            store_word(packet + 4, 0);
            quad(d, 0x60000000ull, 0);
            store_pointer(packet + 0, d + 16);
        }
    }
    {
        const unsigned f = load_word(polyset + 36);
        void* const list0 = glistp;
        unsigned char* const end = load_pointer(packet + 0);
        glistp = end;
        drawAddToList__FPviP8_textureP11_drawRecordi(list0, (f & 0x200) != 0 ? 4 : 0, texture, nullptr, 0);
    }
}
