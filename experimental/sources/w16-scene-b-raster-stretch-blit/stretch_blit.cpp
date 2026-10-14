// Observed: StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef at 0x0110bf08 is
// 2680 bytes. Arguments: the raster (this); an int texture id; unsigned flags; four
// const pairs of ints (a = destination origin, b = destination size, c = source
// origin, d = source size; pointers in a3, t0, t1, t2); the blend mode (t3); and the
// float alpha (f12). Offsets are bytes from the raster; "word" is a 32-bit address.
//   1. The texture record is the first word of the 12-byte entry at (word +19388) +
//      12 * id. A pending-upload check: when word +19532 is not 1 it calls
//      UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, raster + 19492).
//   2. The material is the word at +17848. It stores id at material +48 and the blend
//      mode at material +60. Bit 2 of flags (mask 0x4) selects SetDither(material, 0),
//      otherwise SetDither(material, 1). If id differs from the raster word +17852, or
//      the blend mode from +17856, it stores the blend mode at +17856, 1 at material
//      +32 and the id at +17852.
//   3. AddMaterial__8VIRasteriPP10_sceDmaTag(raster, material word, raster + 19492) and
//      BlitBegin__8VIRasterPP10_sceDmaTag(raster, raster + 19492); a negative result
//      from either returns -1.
//   4. It writes four 64-byte vertex records at the output pointer in the raster word
//      +52, in the order (a), (a.x + b.x, a.y), (a.x + b.x, a.y + b.y), (a.x, a.y + b.y),
//      incrementing the raster word +48 after each. A vertex record holds, as floats:
//      +0 (x - R[17732] + 0.5) * R[17740]; +4 (y - R[17736] + 0.5) * -R[17744]; +8 0;
//      +12 1; +16/+20 the texture coordinate; +24 1; +28 0; +32..+47 zeros; +48..+60
//      1, 1, 1, alpha. R[n] is the raster float at +n. The texture coordinate scales
//      the source integers by 1/width and 1/height (texture record ints +8 and +12).
//      The low two flag bits choose which source corner each vertex takes: 0 =
//      (c) to (c + d) in order; 1 mirrors horizontally; 2 mirrors vertically; 3 both.
//   5. The output pointer (+52) advances by 256. BlitEnd__8VIRasterPP10_sceDmaTag
//      (raster, raster + 19492): a negative result returns -1. Otherwise material +48
//      is set to -1, FlushDMABufferIfFull__8VIRaster(raster) is called and it returns 0.
// Inferred: a screen-space textured quad draw (stretching the source rectangle to the
// destination one) in the batched blit path, in contrast with the fan-based text path.
// Unknown: the VIRaster field names, the +32/+17852/+17856 state's readers, the flag
// meanings other than the dither bit and the mirror bits, and VIAlphaBlendMode values.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

namespace {
struct Vect2I {
    int x;
    int y;
};

float rf(const unsigned char* raster, int offset) { return *reinterpret_cast<const float*>(raster + offset); }

void put_vertex(unsigned char* raster, int index, int x, int y, float u, float v, float alpha) {
    float* p = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(raster + 52)) + index * 64);
    p[0] = (static_cast<float>(x) - rf(raster, 17732) + 0.5f) * rf(raster, 17740);
    p[1] = (static_cast<float>(y) - rf(raster, 17736) + 0.5f) * -rf(raster, 17744);
    p[2] = 0.0f;
    p[3] = 1.0f;
    p[4] = u;
    p[5] = v;
    p[6] = 1.0f;
    p[7] = 0.0f;
    p[8] = 0.0f;
    p[9] = 0.0f;
    p[10] = 0.0f;
    p[11] = 0.0f;
    p[12] = 1.0f;
    p[13] = 1.0f;
    p[14] = 1.0f;
    p[15] = alpha;
    *reinterpret_cast<int*>(raster + 48) += 1;
}
}

extern "C" {
int UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tagSlot);
void SetDither__10VIMateriali(unsigned char* material, int enable);
int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* raster, int material, unsigned char* tagSlot);
int BlitBegin__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tagSlot);
int BlitEnd__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tagSlot);
int FlushDMABufferIfFull__8VIRaster(unsigned char* raster);

int StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef(unsigned char* raster, int id, unsigned flags, const Vect2I* a,
                                                                const Vect2I* b, const Vect2I* c, const Vect2I* d, int blend,
                                                                float alpha) {
    float u_scale = 0.0f;
    float v_scale = 0.0f;
    unsigned char* texture = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(raster + 19388)) + id * 12));
    unsigned char* material = at(*reinterpret_cast<unsigned*>(raster + 17848));
    unsigned char* tag_slot = raster + 19492;
    if (*reinterpret_cast<int*>(raster + 19532) != 1)
        UploadRasterMicro__8VIRasterPP10_sceDmaTag(raster, tag_slot);
    *reinterpret_cast<int*>(material + 48) = id;
    *reinterpret_cast<int*>(material + 60) = blend;
    if (flags & 4)
        SetDither__10VIMateriali(material, 0);
    else
        SetDither__10VIMateriali(material, 1);
    if (id != *reinterpret_cast<int*>(raster + 17852)) {
        *reinterpret_cast<int*>(raster + 17856) = blend;
        *reinterpret_cast<int*>(material + 32) = 1;
        *reinterpret_cast<int*>(raster + 17852) = id;
    } else if (blend != *reinterpret_cast<int*>(raster + 17856)) {
        *reinterpret_cast<int*>(raster + 17856) = blend;
        *reinterpret_cast<int*>(material + 32) = 1;
        *reinterpret_cast<int*>(raster + 17852) = id;
    }
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(raster, static_cast<int>(*reinterpret_cast<unsigned*>(raster + 17848)), tag_slot) < 0)
        return -1;
    if (BlitBegin__8VIRasterPP10_sceDmaTag(raster, tag_slot) < 0)
        return -1;
    int ax = a->x;
    int ay = a->y;
    u_scale = 1.0f / static_cast<float>(*reinterpret_cast<int*>(texture + 8));
    v_scale = 1.0f / static_cast<float>(*reinterpret_cast<int*>(texture + 12));
    // source corners: left/right and top/bottom texel positions for the unflipped case
    int left = c->x;
    int right = c->x + d->x;
    int top = c->y;
    int bottom = c->y + d->y;
    unsigned mode = flags & 3;
    if (mode == 1 || mode == 3) {
        int t = left;
        left = right;
        right = t;
    }
    if (mode == 2 || mode == 3) {
        int t = top;
        top = bottom;
        bottom = t;
    }
    put_vertex(raster, 0, ax, ay, static_cast<float>(left) * u_scale, static_cast<float>(top) * v_scale, alpha);
    int bx = ax + b->x;
    put_vertex(raster, 1, bx, ay, static_cast<float>(right) * u_scale, static_cast<float>(top) * v_scale, alpha);
    int by = ay + b->y;
    put_vertex(raster, 2, bx, by, static_cast<float>(right) * u_scale, static_cast<float>(bottom) * v_scale, alpha);
    put_vertex(raster, 3, ax, by, static_cast<float>(left) * u_scale, static_cast<float>(bottom) * v_scale, alpha);
    *reinterpret_cast<unsigned*>(raster + 52) += 256;
    if (BlitEnd__8VIRasterPP10_sceDmaTag(raster, tag_slot) < 0)
        return -1;
    *reinterpret_cast<int*>(material + 48) = -1;
    FlushDMABufferIfFull__8VIRaster(raster);
    return 0;
}
}
