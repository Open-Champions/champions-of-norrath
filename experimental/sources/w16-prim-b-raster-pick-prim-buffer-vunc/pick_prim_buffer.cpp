// Observed: PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal at
// 0x0111d740 is 1816 bytes. Arguments: the raster (a0), a pick segment (a1), a prim buffer pb
// (a2) and a material palette pal (a3). Offsets are bytes.
// Setup: three local float triples P, A, B and three 4-byte colours cP, cA, cB start at 0.0 /
// (0,0,0,255); a flip flag and the result start at 0. If the group count pb[64] is not positive
// the result (0) is returned. Otherwise it walks pb[64] groups. A group is the 16-byte record at
// (pb[88]) + g*16: word 0 is a material index m and word 4 an entry count; the palette word used
// for the group is the word at (pal[12]) + m*4. A running slot pointer starts at pb[96] and a
// running byte offset o starts at 0 (neither resets between groups). For each of the group's
// entries, the 28-byte record e at (pb[92]) + o:
//   * Cull__8VIRasterR13VIPickSegmentR6VIBBox(raster, segment, e + 4); when it returns non-zero the
//     entry is skipped: the slot pointer advances by e[0] * 64 + 48 and o by 28.
//   * otherwise the slot pointer advances by 32, the flip flag is cleared, and two vertices are
//     read, each from a 64-byte stride: position A = the three floats at slot+0 and colour cA = the
//     four floats at slot+48, each converted as (unsigned char)(unsigned)(f * 255.0f + 0.5f); then
//     B likewise from the next 64 bytes. Then e[0] - 2 further vertices follow (a do-while: it is
//     not checked for zero before the first pass, and a count below 2 would run for very long).
//     Each pass: P = A, A = B, cP = cA, cA = cB, then B and cB are read from the next 64-byte
//     vertex. With flip clear it calls Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23(raster,
//     segment, palette word, P, A, B); with flip set the order is (A, P, B). A non-zero result
//     calls CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25 with the same three
//     points and their three colours in matching order (cP, cA, cB, or cA, cP, cB when flipped)
//     and sets the result to 1. Flip toggles after each pass. After the entry the slot pointer
//     advances by 16 and o by 28.
// It returns the result: 1 when any triangle was hit, else 0.
// Inferred: a triangle-strip pick over a float-vertex prim buffer (position in the first slot,
// colour floats in the fourth slot of each 64-byte vertex, matching Vertex__12VIPrimBufferR7VIVect3),
// with the flip flag keeping the winding consistent along the strip. Unknown: the exact meaning
// of the Pick and CalcHitColor arguments (taken from their mangled names), the layout of
// the palette beyond words 12 and the 16 trailing bytes per entry, and the group record's other words.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Cull__8VIRasterR13VIPickSegmentR6VIBBox(void* raster, void* segment, const void* box);
int Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23(void* raster, void* segment, int material, const float* a, const float* b, const float* c);
void CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25(void* raster, void* segment, const float* a, const float* b, const float* c, const unsigned char* colorA, const unsigned char* colorB, const unsigned char* colorC);
}

namespace {
inline unsigned char toByte(float f) {
    return static_cast<unsigned char>(static_cast<unsigned>(f * 255.0f + 0.5f));
}

inline void readVertex(const unsigned char* slot, float* position, unsigned char* color) {
    const float* p = reinterpret_cast<const float*>(slot);
    const float* c = reinterpret_cast<const float*>(slot + 48);
    position[0] = p[0];
    position[1] = p[1];
    position[2] = p[2];
    color[0] = toByte(c[0]);
    color[1] = toByte(c[1]);
    color[2] = toByte(c[2]);
    color[3] = toByte(c[3]);
}

inline void copyColor(unsigned char* to, const unsigned char* from) {
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    to[3] = from[3];
}
}

extern "C" {
int PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char* raster, unsigned char* segment,
                                                                                unsigned char* pb, unsigned char* pal) {
    float p[3] = {0.0f, 0.0f, 0.0f};
    float a[3] = {0.0f, 0.0f, 0.0f};
    float b[3] = {0.0f, 0.0f, 0.0f};
    unsigned char cp[4] = {0, 0, 0, 255};
    unsigned char ca[4] = {0, 0, 0, 255};
    unsigned char cb[4] = {0, 0, 0, 255};
    int flip = 0;
    int result = 0;
    int groups = *reinterpret_cast<int*>(pb + 64);
    if (groups > 0) {
        unsigned char* slot = at(*reinterpret_cast<unsigned*>(pb + 96));
        int offset = 0;
        for (int g = 0; g < groups; ++g) {
            unsigned char* record = at(*reinterpret_cast<unsigned*>(pb + 88)) + g * 16;
            int material = *reinterpret_cast<int*>(record);
            int count = *reinterpret_cast<int*>(record + 4);
            unsigned paletteWord = *reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(pal + 12)) + material * 4);
            for (; count != 0; --count) {
                unsigned char* entry = at(*reinterpret_cast<unsigned*>(pb + 92)) + offset;
                if (Cull__8VIRasterR13VIPickSegmentR6VIBBox(raster, segment, entry + 4) != 0) {
                    slot += *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(pb + 92)) + offset) * 64 + 48;
                    offset += 28;
                    continue;
                }
                slot += 32;
                flip = 0;
                readVertex(slot, a, ca);
                slot += 64;
                readVertex(slot, b, cb);
                int more = *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(pb + 92)) + offset) - 2;
                slot += 64;
                if (more != 0) {
                    do {
                        p[0] = a[0];
                        p[1] = a[1];
                        p[2] = a[2];
                        a[0] = b[0];
                        a[1] = b[1];
                        a[2] = b[2];
                        copyColor(cp, ca);
                        copyColor(ca, cb);
                        readVertex(slot, b, cb);
                        slot += 64;
                        if (flip == 0) {
                            if (Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23(raster, segment, static_cast<int>(paletteWord), p, a, b) != 0) {
                                CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25(raster, segment, p, a, b, cp, ca, cb);
                                result = 1;
                            }
                        } else {
                            if (Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23(raster, segment, static_cast<int>(paletteWord), a, p, b) != 0) {
                                CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25(raster, segment, a, p, b, ca, cp, cb);
                                result = 1;
                            }
                        }
                        flip = 1 - flip;
                    } while (--more != 0);
                }
                offset += 28;
                slot += 16;
            }
        }
    }
    return result;
}
}
