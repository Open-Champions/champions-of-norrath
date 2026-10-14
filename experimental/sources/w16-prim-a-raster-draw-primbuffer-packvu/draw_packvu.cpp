// Observed: DrawPrimBufferPackVU__8VIRasteriifUi at 0x011117e8 is 476 bytes. Arguments: the raster R (this), integer a, integer b, float s (passed in a float register) and flags f. Offsets are bytes. P = the first
// word of the 12-byte entry a in the array at (R + 19328), read before the first call. It calls
// FlushDMABufferIfFull__8VIRaster(R); stores the float argument s at scratchpad offset 0x228; it then copies four floats from P + 48 to scratchpad
// offset 0x210; if the word at R + 17888 is nonzero it calls CalcMatrices__8VIRaster(R); it then
// always calls UploadMatrices__8VIRasterPP10_sceDmaTag(R, R + 19492). With n = word at P + 64
// (material count) and result = 0, for each i in 0 .. n - 1:
//   * id = word 0 of the 16-byte record at (pointer at P + 88) + i * 16; the material
//     m = word id of the array at (word 12 of the object that is the first word of the 12-byte entry b
//     in the array at R + 19436);
//   * if the word at m + 60 is nonzero then result = 1 and the material is drawn only when f & 2
//     is set; if it is zero the material is drawn only when f & 1 is set;
//   * drawing calls AddMaterial__8VIRasteriPP10_sceDmaTag(R, m, R + 19492), then, with t = the pointer stored at R + 19492 and
//     the record's words 2 and 3 (offsets 8 and 12): t byte 3 = 0x30, t word 1 = (pointer at
//     P + 96) + word2 * 16, t halfword 0 = word3 (low 16 bits), and R + 19492 advances by 16.
// After the loop it calls FlushDMABufferIfFull__8VIRaster(R) again and returns result.
// Inferred: R + 19492 is the current write position in a DMA tag list and the 16 bytes written there
// are an EE DMA tag (halfword count at 0, ID byte 0x30 at 3, address at 4); P is a packed primitive
// buffer, the record words 2/3 are the offset and length (in 16-byte units) of one material's run.
// Scratchpad offsets are relative to 0x70000000. Unknown: the real field names, what the word at
// m + 60 flags (probably blending), the meaning of a, b and f beyond their uses here, and what the
// entry-b object is.
extern "C" {
void FlushDMABufferIfFull__8VIRaster(unsigned char* raster);
void CalcMatrices__8VIRaster(unsigned char* raster);
void UploadMatrices__8VIRasterPP10_sceDmaTag(unsigned char* raster, unsigned char* tag_cursor);
void AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char* raster, unsigned char* material, unsigned char* tag_cursor);
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
int DrawPrimBufferPackVU__8VIRasteriifUi(unsigned char* raster, int a, int b, float scalar, unsigned flags) {
    unsigned char* buffer = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(raster + 19328)) + a * 12));
    int result = 0;
    FlushDMABufferIfFull__8VIRaster(raster);
    *reinterpret_cast<float*>(scratch(0x228)) = scalar;
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
    UploadMatrices__8VIRasterPP10_sceDmaTag(raster, raster + 19492);
    int count = *reinterpret_cast<int*>(buffer + 64);
    if (count > 0) {
        int i = 0;
        do {
            int record_offset = i * 16;
            unsigned char* set = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(raster + 19436)) + b * 12));
            unsigned id = *reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(buffer + 88)) + record_offset);
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
                unsigned char* record = at(*reinterpret_cast<unsigned*>(buffer + 88)) + record_offset;
                unsigned char* tag = at(*reinterpret_cast<unsigned*>(raster + 19492));
                unsigned length = *reinterpret_cast<unsigned*>(record + 12);
                tag[3] = 48;
                *reinterpret_cast<unsigned*>(tag + 4) = *reinterpret_cast<unsigned*>(buffer + 96) + *reinterpret_cast<unsigned*>(record + 8) * 16;
                tag = at(*reinterpret_cast<unsigned*>(raster + 19492));
                *reinterpret_cast<unsigned short*>(tag) = static_cast<unsigned short>(length);
                *reinterpret_cast<unsigned*>(raster + 19492) = *reinterpret_cast<unsigned*>(raster + 19492) + 16;
                count = *reinterpret_cast<int*>(buffer + 64);
            }
        } while (i < count);
    }
    FlushDMABufferIfFull__8VIRaster(raster);
    return result;
}
}
