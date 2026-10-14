// Observed: PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal at
// 0x0111de58 is 1272 bytes. Arguments and walk are those of
// PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal (group records at
// pb[88], 28-byte entries at pb[92], palette word at (pal[12]) + material*4, Cull, then a strip of
// Pick / CalcHitColor calls with a flip flag, result 1 on any hit), with these differences: each
// vertex is a 32-byte stride, the entry header before the first vertex is 32 bytes, and the entry
// trailer is 16 bytes (a skipped entry advances the slot pointer by e[0] * 32 + 48). A vertex at
// slot v has its position as three signed halfwords at v+4, v+6, v+8, each converted to float and
// multiplied by the float at pb[48], and its colour as the four bytes at v+28..31 each shifted
// left by one (stored back as a byte).
// Inferred: the packed-vertex counterpart of the float strip pick; the halfword layout matches
// Vertex__12VIPrimBufferR10VIVect3S16 types 2 and 4 (position at +4, colour at +28 of a two-slot
// vertex, colour stored halved) and pb[48] is the first SetPacking__12VIPrimBufferiii scale.
// Unknown: the same items as the float variant (argument roles of Pick and CalcHitColor, other
// palette and group-record fields), and whether a count below 2 can occur (it would run very long).
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
inline void readVertex(const unsigned char* slot, const unsigned char* pb, float* position, unsigned char* color) {
    const short* p = reinterpret_cast<const short*>(slot + 4);
    float scale = *reinterpret_cast<const float*>(pb + 48);
    position[0] = static_cast<float>(p[0]) * scale;
    position[1] = static_cast<float>(p[1]) * scale;
    position[2] = static_cast<float>(p[2]) * scale;
    const unsigned char* c = slot + 28;
    color[0] = static_cast<unsigned char>(c[0] << 1);
    color[1] = static_cast<unsigned char>(c[1] << 1);
    color[2] = static_cast<unsigned char>(c[2] << 1);
    color[3] = static_cast<unsigned char>(c[3] << 1);
}

inline void copyColor(unsigned char* to, const unsigned char* from) {
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    to[3] = from[3];
}
}

extern "C" {
int PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal(unsigned char* raster, unsigned char* segment,
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
                    slot += *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(pb + 92)) + offset) * 32 + 48;
                    offset += 28;
                    continue;
                }
                slot += 32;
                flip = 0;
                readVertex(slot, pb, a, ca);
                slot += 32;
                readVertex(slot, pb, b, cb);
                int more = *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(pb + 92)) + offset) - 2;
                slot += 32;
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
                        readVertex(slot, pb, b, cb);
                        slot += 32;
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
