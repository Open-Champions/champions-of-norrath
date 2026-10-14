// Observed modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl
// at 0x0016bb10, 1360 bytes. Arguments: model header, texture, an int forwarded to
// drawAddToList, a Matrix34, a float scale, an animation state (may be null) and a
// 32-bit segment mask. Returns nothing. It appends display-list packets at the
// cursor glistp (0x004844c8) in the style of modelDraw: a local two-word packet
// {cursor, last tag}, DMA CNT tags 0x10000000 | quadword count, VIF DIRECT words
// 0x50000000 | quadword count, and A+D GIF tags 0x1000000000008000 | loop count with
// register selector 14.
//
// 1. M = matMul(object matrix, worldCharViewMatrix) as 16 floats on the stack.
//    Then M[2], M[6], M[10] and M[14] each gain M[3], M[7], M[11] and M[15]
//    multiplied by scale and by 2^42 (0x54800000).
// 2. Packet one: a VIF flush word 0x11000000 follows the DMA tag, then zero bytes
//    up to 12 modulo 16, the DIRECT word and the A+D tag. Five entries, data first:
//    (0, 63) (0, 6) (the 64-bit value at model + 24, 66) (0x51001, 71) (0, 69).
//    After them: the UNPACK word 0x6c04c000, the 16 floats of M, and the word
//    0x14000002. Then lightSetupAmbientOnly(packet) runs, the cursor is padded with
//    zero bytes to a quadword, and the DMA tag becomes ((units - 2) >> 1) |
//    0x10000000 over the span from the tag to the cursor (read from the packet's
//    second word, which the callee may have changed).
// 3. With no animation state the packet's second word becomes the cursor and the
//    pair (0x10000000, 0x1400001e) is written there; otherwise
//    animLoadBoneMatricies(packet, anim) runs, the second word becomes the cursor
//    after it, and the pair is (0x10000000, 0x1400000c). The cursor moves 16 bytes.
// 4. Segment runs, as in modelDraw: the clamped level index is min(0, byte(model +
//    38) - 1); its table at model + 40 holds byte(model + 18) + 1 addresses per
//    level. The mask has bit byte(model + 18) cleared. Walking the segments, each
//    maximal run of set mask bits is covered at the next clear bit by one 16-byte
//    REF tag (0x30000000, run start address in the upper word, signed (end - start)
//    >> 4 in the low word, zero second word) when the run is non-empty; the tag is
//    recorded as the packet's second word.
// 5. Packet two: the same opening without a flush word, one entry (1, 69), the
//    counts patched in, and the DMA tag closed: tag id 1 becomes id 6 in place;
//    otherwise the second word is cleared and the 16-byte tag 0x60000000 is appended
//    (that branch cannot run: the tag id is always 1).
//    DMA counts are signed 32-bit values widened to 64 bits before the id is ORed
//    in, so a negative count would set the upper word.
//    glistp becomes the cursor and drawAddToList(old glistp, int, texture, 0, 0)
//    is called.
// Inferred: register 66 is ALPHA, 71 TEST and 69 DTHE in the GS register map; the
// effect model reads those from the model. Unknown: the meaning of the model bytes
// and of the 2^42 constant's role.
struct Packet {
    unsigned char* cursor;
    unsigned char* last;
};

struct AnimationState;
struct Texture;
struct ModelHeader;
struct Matrix34;
struct Matrix44;

extern "C" {
extern unsigned char* glistp;
extern float worldCharViewMatrix[12];

void matMul__FP8Matrix44P8Matrix34T0(Matrix44* out, Matrix34* a, Matrix34* b);
void lightSetupAmbientOnly__FP6Packet(Packet*);
void animLoadBoneMatricies__FP6PacketP14AnimationState(Packet*, AnimationState*);
void drawAddToList__FPviP8_textureP11_drawRecordi(void*, int, Texture*, void*, int);
}

namespace {
inline void store64(unsigned char* where, unsigned long long value) { *reinterpret_cast<unsigned long long*>(where) = value; }
inline void store32(unsigned char* where, unsigned value) { *reinterpret_cast<unsigned*>(where) = value; }
inline unsigned long long load64(const unsigned char* where) { return *reinterpret_cast<const unsigned long long*>(where); }
inline unsigned long long sext(int value) { return static_cast<unsigned long long>(static_cast<long long>(value)); }
}

extern "C" void modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl(
    const unsigned char* model, Texture* texture, int value, Matrix34* object, float scale, AnimationState* anim,
    unsigned long mask) {
    Packet packet;
    packet.cursor = glistp;
    float m[16];
    matMul__FP8Matrix44P8Matrix34T0(reinterpret_cast<Matrix44*>(m), object, reinterpret_cast<Matrix34*>(worldCharViewMatrix));
    const float big = __builtin_bit_cast(float, 0x54800000u);
    const float a3 = m[3] * scale * big;
    const float a7 = m[7] * scale * big;
    const float a11 = m[11] * scale * big;
    const float a15 = m[15] * scale * big;
    m[2] = m[2] + a3;
    m[6] = m[6] + a7;
    m[10] = m[10] + a11;
    m[14] = m[14] + a15;

    // packet one
    unsigned char* dma = packet.cursor;
    packet.last = dma;
    unsigned char* at = dma + 8;
    store32(at, 0x11000000u);
    at += 4;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(at) & 0xfu) != 12u) *at++ = 0;
    unsigned char* vif = at;
    unsigned char* tag = at + 4;
    packet.cursor = tag;
    store64(tag, 0x1000000000008000ull);
    store64(tag + 8, 14ull);
    unsigned char* e = packet.cursor;
    store64(e + 16, 0ull);                                    store64(e + 24, 63ull);
    store64(e + 32, 0ull);                                    store64(e + 40, 6ull);
    store64(e + 48, load64(model + 24));                      store64(e + 56, 66ull);
    store64(e + 64, 0x51001ull);                              store64(e + 72, 71ull);
    store64(e + 80, 0ull);                                    store64(e + 88, 69ull);
    store32(e + 96, 0x6c04c000u);
    for (int i = 0; i < 16; ++i) *reinterpret_cast<float*>(e + 100 + 4 * i) = m[i];
    store32(e + 164, 0x14000002u);
    packet.cursor = e + 168;
    {
        unsigned char* gifEnd = e + 96;
        store64(tag, load64(tag) | sext((static_cast<int>(gifEnd - tag) / 8 - 2) >> 1));
        store32(vif, 0x50000000u | static_cast<unsigned>((static_cast<int>(gifEnd - vif) - 4) >> 4));
    }
    lightSetupAmbientOnly__FP6Packet(&packet);
    {
        unsigned char* cursor = packet.cursor;
        while ((reinterpret_cast<__UINTPTR_TYPE__>(cursor) & 0xfu) != 0u) *cursor++ = 0;
        packet.cursor = cursor;
        unsigned char* last = packet.last;
        const int units = ((static_cast<int>(cursor - last) >> 3) - 2) >> 1;
        store64(last, sext(units) | 0x10000000ull);
    }

    unsigned long long headWord;
    if (anim == nullptr) {
        packet.last = packet.cursor;
        headWord = 0x1400001eull;
    } else {
        animLoadBoneMatricies__FP6PacketP14AnimationState(&packet, anim);
        packet.last = packet.cursor;
        headWord = 0x1400000cull;
    }
    {
        unsigned char* where = packet.cursor;
        store64(where, 0x10000000ull);
        store64(where + 8, headWord);
        packet.cursor = where + 16;
    }

    // segment runs
    const int segments = static_cast<signed char>(model[18]);
    int level = static_cast<signed char>(model[38]) - 1;
    if (level > 0) level = 0;
    const unsigned char* table = model + 40 + level * (segments * 4 + 4);
    long long bits = static_cast<long long>(mask) & ~(1ll << segments);
    if (segments >= 0) {
        unsigned start = *reinterpret_cast<const unsigned*>(table);
        for (int i = 0; i <= segments; ++i, table += 4) {
            const bool clear = (static_cast<int>(bits) & 1) == 0;
            bits >>= 1;
            if (clear) {
                const unsigned end = *reinterpret_cast<const unsigned*>(table);
                if (start != end) {
                    const unsigned long long ref = (static_cast<unsigned long long>(start) << 32) |
                                                   sext(static_cast<int>(end - start) >> 4) | 0x30000000ull;
                    unsigned char* where = packet.cursor;
                    packet.cursor = where + 16;
                    store64(where, ref);
                    store64(where + 8, 0ull);
                    packet.last = where;
                }
                start = *reinterpret_cast<const unsigned*>(table + 4);
            }
        }
    }

    // packet two
    packet.last = packet.cursor;
    {
        unsigned char* start2 = packet.cursor;
        unsigned char* at2 = start2 + 8;
        packet.cursor = at2;
        while ((reinterpret_cast<__UINTPTR_TYPE__>(at2) & 0xfu) != 12u) *at2++ = 0;
        unsigned char* vif2 = at2;
        unsigned char* gif2 = at2 + 4;
        packet.cursor = gif2;
        store64(gif2, 0x1000000000008000ull);
        store64(gif2 + 8, 14ull);
        unsigned char* entry2 = packet.cursor + 16;
        packet.cursor = packet.cursor + 32;
        store64(entry2, 1ull);
        store64(entry2 + 8, 69ull);
        unsigned char* end2 = packet.cursor;
        unsigned char* last2 = packet.last;
        const int units2 = ((static_cast<int>(end2 - last2) >> 3) - 2) >> 1;
        const unsigned long long closeTag = sext(units2) | 0x10000000ull;
        const int gifLoop = ((static_cast<int>(end2 - gif2) >> 3) - 2) >> 1;
        store64(gif2, load64(gif2) | sext(gifLoop));
        store32(vif2, 0x50000000u | static_cast<unsigned>((static_cast<int>(end2 - vif2) - 4) >> 4));
        store64(last2, closeTag);
        if (((closeTag >> 28) & 7u) == 1u) {
            store64(last2, (closeTag & 0xffffffff8fffffffull) | 0x60000000ull);
        } else {
            packet.last = nullptr;
            store64(end2, 0x60000000ull);
            store64(end2 + 8, 0ull);
            packet.cursor = end2 + 16;
        }
    }
    unsigned char* old = glistp;
    glistp = packet.cursor;
    drawAddToList__FPviP8_textureP11_drawRecordi(old, value, texture, nullptr, 0);
}
