// Observed: DrawPrimBufferPackVUNC__8VIRasteriiiUi at 0x011119c8 is 672 bytes. Arguments: the raster R (this), integers a, b and c, and
// flags f (fifth argument, in a register beyond the fourth). Offsets are bytes. P = the first word of the
// 12-byte entry a in the array at (R + 19328) and Q = the object whose pointer is the first word of
// the 12-byte entry b in the array at (R + 19364), both read before the first call. result = 0. It calls
// FlushDMABufferIfFull__8VIRaster(R). If the word at Q + 12 is not equal to a, it returns -1 at once
// (without the final flush); otherwise it copies four floats from P + 48 to scratchpad offset 0x210;
// if the word at R + 17888 is nonzero it calls CalcMatrices__8VIRaster(R); then
//   R + 17864 is cleared; if the byte at scratchpad 0x222 is nonzero a sphere is built -- centre = three
//   floats from scratchpad 0x190, radius = the float at 0x1cc times 1 / sqrt(x*x + y*y + z*z) of the three
//   floats at 0x100 (computed with the hardware rsqrt.s instruction, numerator 1.0) -- and
//   Intersects__C6VIBBoxRC8VISphere(P + 20, sphere) is called; if it returns nonzero, R + 17864 = 1.
// and UploadMatrices__8VIRasterPP10_sceDmaTag(R, R + 19492) is always called. With n = word at
// Q + 16, for each i in 0 .. n - 1:
//   * id = word 0 of the 8-byte record at (pointer at Q + 36) + i * 8; the material m = word id of
//     the array at (word 12 of the object that is the first word of the 12-byte entry c in the array at
//     R + 19436);
//   * if the word at m + 60 is nonzero then result = 1 and the material is drawn only when f & 2 is
//     set; if it is zero the material is drawn only when f & 1 is set;
//   * drawing calls AddMaterial__8VIRasteriPP10_sceDmaTag(R, m, R + 19492), then, with t = the pointer stored at R + 19492 and
//     the record's word 1: t byte 3 = 0x50, t word 1 = (pointer at Q + 40) + word1 * 16, t halfword 0
//     = 0, and R + 19492 advances by 16.
// After the loop it calls FlushDMABufferIfFull__8VIRaster(R) again and returns result.
// Inferred: Q is a second kind of packed run list whose record word 1 is an offset in 16-byte units into
// the data at Q + 40; the check against a ties Q to the primitive buffer a. Unknown: the real names,
// the meaning of Q + 12 (it is compared with a here), and the zero count in the tag (data qword count
// is not given).
extern "C" {
void FlushDMABufferIfFull__8VIRaster(unsigned char* raster);
void CalcMatrices__8VIRaster(unsigned char* raster);
void UploadMatrices__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tag_cursor);
void AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* raster, unsigned char* material, unsigned char* tag_cursor);
int Intersects__C6VIBBoxRC8VISphere(const unsigned char* box, const float* sphere);
}

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned char* norrath_scratchpad(unsigned offset);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
static inline unsigned char* scratch(unsigned offset) { return norrath_scratchpad(offset); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
static inline unsigned char* scratch(unsigned offset) { return reinterpret_cast<unsigned char*>(0x70000000u + offset); }
#endif

extern "C" {
int DrawPrimBufferPackVUNC__8VIRasteriiiUi(unsigned char* raster, int a, int b, int c, unsigned flags) {
    unsigned char* object = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(raster + 19364)) + b * 12));
    unsigned char* buffer = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(raster + 19328)) + a * 12));
    FlushDMABufferIfFull__8VIRaster(raster);
    int result = 0;
    if (*reinterpret_cast<int*>(object + 12) != a)
        return -1;
    {
        const float* source = reinterpret_cast<const float*>(buffer + 48);
        float* target = reinterpret_cast<float*>(scratch(0x210));
        target[0] = source[0];
        target[1] = source[1];
        target[2] = source[2];
        target[3] = source[3];
    }
    if (*reinterpret_cast<int*>(raster + 17888) != 0)
        CalcMatrices__8VIRaster(raster);
    *reinterpret_cast<int*>(raster + 17864) = 0;
    if (*scratch(0x222) != 0) {
        float sphere[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        const float* axis = reinterpret_cast<const float*>(scratch(0x100));
        float length_squared = axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
        float one = 1.0f;
        float scale;
#if defined(__mips__)
        __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(scale) : "f"(one), "f"(length_squared));
#else
        scale = one / __builtin_sqrtf(length_squared);
#endif
        sphere[3] = *reinterpret_cast<const float*>(scratch(0x1cc)) * scale;
        const float* centre = reinterpret_cast<const float*>(scratch(0x190));
        sphere[0] = centre[0];
        sphere[1] = centre[1];
        sphere[2] = centre[2];
        if (Intersects__C6VIBBoxRC8VISphere(buffer + 20, sphere))
            *reinterpret_cast<int*>(raster + 17864) = 1;
    }
    UploadMatrices__8VIRasterPP10_sceDmaTag(raster, raster + 19492);
    int count = *reinterpret_cast<int*>(object + 16);
    if (count > 0) {
        int i = 0;
        do {
            int record_offset = i * 8;
            unsigned char* set = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(raster + 19436)) + c * 12));
            unsigned id = *reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(object + 36)) + record_offset);
            unsigned char* material = at(reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(set + 12)))[id]);
            ++i;
            bool draw;
            if (*reinterpret_cast<int*>(material + 60) == 0) {
                draw = (flags & 1) != 0;
            } else {
                result = 1;
                draw = (flags & 2) != 0;
            }
            if (draw) {
                AddMaterial__8VIRasteriPP10_sceDmaTag(raster, material, raster + 19492);
                unsigned char* record = at(*reinterpret_cast<unsigned*>(object + 36)) + record_offset;
                unsigned char* tag = at(*reinterpret_cast<unsigned*>(raster + 19492));
                tag[3] = 80;
                *reinterpret_cast<unsigned*>(tag + 4) = *reinterpret_cast<unsigned*>(object + 40) + *reinterpret_cast<unsigned*>(record + 4) * 16;
                tag = at(*reinterpret_cast<unsigned*>(raster + 19492));
                *reinterpret_cast<unsigned short*>(tag) = 0;
                *reinterpret_cast<unsigned*>(raster + 19492) = *reinterpret_cast<unsigned*>(raster + 19492) + 16;
                count = *reinterpret_cast<int*>(object + 16);
            }
        } while (i < count);
    }
    FlushDMABufferIfFull__8VIRaster(raster);
    return result;
}
}
