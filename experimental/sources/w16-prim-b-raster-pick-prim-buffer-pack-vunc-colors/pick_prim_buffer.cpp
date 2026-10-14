// Observed: PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal
// at 0x0111e938 is 1448 bytes. Arguments: the raster (a0), a pick segment (a1), a prim buffer pb
// (a2), a colour buffer cb (a3) and a material palette (t0). It is the packed strip pick
// PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal with the vertex
// colours taken from a separate array instead of the vertex slots: positions are the signed
// halfwords at v+4, v+6, v+8 of each 32-byte vertex times the float at pb[48], as there, but the
// colour of each vertex is four bytes read from a cursor into the buffer at cb[44], each shifted
// left by one. For an entry of n vertices (n = e[0] of the 28-byte record) the cursor c starts at
// the entry's block start; vertex k (from 0) uses the four bytes at c + 8 + 4*k, so the first
// colour sits eight bytes into the block. After the entry, processed or culled, the block start
// advances by q*16 plus 16 more when (n + 4) & 3 is non-zero, where q = (n + 4) / 4 (C division);
// the original reaches the same total for processed entries through a running counter that adds
// 16 each time four colours have been read. Slot pointer, group walk, Cull, Pick, CalcHitColor
// and the flip flag behave exactly as in the non-colour-buffer sibling; the result is 1 when
// any triangle was hit, else 0.
// Inferred: the colour array is organised in 16-byte blocks of four colours with two leading
// colours per entry (the entry header) and rounding up to a block; this is read from the cursor
// arithmetic only. Unknown: what the first two colours of an entry hold, the buffer's other
// fields, and whether n below 2 can occur (it would run very long).
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
inline void readPosition(const unsigned char* slot, const unsigned char* pb, float* position) {
    const short* p = reinterpret_cast<const short*>(slot + 4);
    float scale = *reinterpret_cast<const float*>(pb + 48);
    position[0] = static_cast<float>(p[0]) * scale;
    position[1] = static_cast<float>(p[1]) * scale;
    position[2] = static_cast<float>(p[2]) * scale;
}

inline void readColor(const unsigned char* c, unsigned char* color) {
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

inline int colorAdvance(int n) {
    int blocks = (n + 4) / 4;
    return blocks * 16 + (((n + 4) & 3) != 0 ? 16 : 0);
}
}

extern "C" {
int PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal(
    unsigned char* raster, unsigned char* segment, unsigned char* pb, unsigned char* cb, unsigned char* pal) {
    float p[3] = {0.0f, 0.0f, 0.0f};
    float a[3] = {0.0f, 0.0f, 0.0f};
    float b[3] = {0.0f, 0.0f, 0.0f};
    unsigned char cp[4] = {0, 0, 0, 255};
    unsigned char ca[4] = {0, 0, 0, 255};
    unsigned char cb4[4] = {0, 0, 0, 255};
    int flip = 0;
    int result = 0;
    unsigned char* colors = at(*reinterpret_cast<unsigned*>(cb + 44));
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
                    int n = *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(pb + 92)) + offset);
                    slot += n * 32 + 48;
                    colors += colorAdvance(n);
                    offset += 28;
                    continue;
                }
                int n = *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(pb + 92)) + offset);
                slot += 32;
                flip = 0;
                readPosition(slot, pb, a);
                readColor(colors + 8, ca);
                slot += 32;
                readPosition(slot, pb, b);
                readColor(colors + 12, cb4);
                slot += 32;
                const unsigned char* colorCursor = colors + 16;
                int more = n - 2;
                if (more != 0) {
                    do {
                        p[0] = a[0];
                        p[1] = a[1];
                        p[2] = a[2];
                        a[0] = b[0];
                        a[1] = b[1];
                        a[2] = b[2];
                        copyColor(cp, ca);
                        copyColor(ca, cb4);
                        readPosition(slot, pb, b);
                        slot += 32;
                        readColor(colorCursor, cb4);
                        colorCursor += 4;
                        if (flip == 0) {
                            if (Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23(raster, segment, static_cast<int>(paletteWord), p, a, b) != 0) {
                                CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25(raster, segment, p, a, b, cp, ca, cb4);
                                result = 1;
                            }
                        } else {
                            if (Pick__8VIRasterR13VIPickSegmentiR7VIVect3N23(raster, segment, static_cast<int>(paletteWord), a, p, b) != 0) {
                                CalcHitColor__8VIRasterR13VIPickSegmentR7VIVect3N22R9VIColor32N25(raster, segment, a, p, b, ca, cp, cb4);
                                result = 1;
                            }
                        }
                        flip = 1 - flip;
                    } while (--more != 0);
                }
                colors += colorAdvance(n);
                offset += 28;
                slot += 16;
            }
        }
    }
    return result;
}
}
