// Observed Init__8VIRasterPviii at 0x01109d40, 816 bytes. Arguments after the
// raster are a pointer that is never read, a width, a height and a flag.
// In order it
//  1. stores the flag as a byte at offset 19536 of the raster;
//  2. converts the four floats at 17788, 17792, 17796 and 17800 to bytes with
//     (value * 255 + 0.5) truncated as an unsigned conversion;
//  3. returns -1 unless the width is 640 and the height is 448;
//  4. clears the word at 17880; if the flag byte is zero calls
//     sceGsResetGraph(0, 1, 2, 0);
//  5. calls sceGsSetDefDBuffDc(&_8VIRaster$DBuffer, 10, 640, 448, 3, 49, 1)
//     and then rewrites words of that buffer (offsets from its start):
//     16 (32-bit) is masked with ~0x1ff and or'd with 0x46; the 64-bit words
//     at 464 and 592 are masked and or'd with 70; at 112, 240, 480 and 608
//     masked and or'd with 140; 96 and 224 masked only; 752 and 384 receive
//     the four bytes packed as r | g<<8 | b<<16 | a<<24 with 0x3f800000 in the
//     upper half; 688, 192, 320 and 560 receive 1;
//  6. FlushCache(0); stores 16.0f at 17772 and 8388608.0f (0x4b000000) at
//     17776; calls InitSPRDoubleBuffer__8VIRaster; stores the width at 17748
//     and the height at 17752; and returns -1 if Init__8VIRaster(raster)
//     returned a negative value, otherwise zero.
// What the buffer words and the stored floats mean is not established here;
// 0x3f800000 in the upper half and 0x1ff masks are as observed.
extern "C" {
struct VIRaster;
struct sceGsDBuffDc;

extern unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");

int sceGsResetGraph(int mode, int interlace, int output, int ffmode);
int sceGsSetDefDBuffDc(sceGsDBuffDc* db, int psm, int width, int height, int ztest, int zpsm,
                       int clear);
void FlushCache(int mode);
void InitSPRDoubleBuffer__8VIRaster(VIRaster* raster);
int Init__8VIRaster(VIRaster* raster);

static unsigned char to_byte(float value) {
    return static_cast<unsigned char>(static_cast<unsigned>(value * 255.0f + 0.5f));
}

static unsigned long long& wide(int offset) {
    return *reinterpret_cast<unsigned long long*>(norrath_raster_dbuffer + offset);
}

int Init__8VIRasterPviii(VIRaster* raster, void* unused, int width, int height, int flag) {
    (void)unused;
    char* base = reinterpret_cast<char*>(raster);
    *reinterpret_cast<unsigned char*>(base + 19536) = static_cast<unsigned char>(flag);
    const float* color = reinterpret_cast<const float*>(base + 17788);
    unsigned char rgba[4];
    rgba[3] = 0xff;
    rgba[0] = 0;
    rgba[1] = 0;
    rgba[2] = 0;
    rgba[0] = to_byte(color[0]);
    rgba[1] = to_byte(color[1]);
    rgba[2] = to_byte(color[2]);
    rgba[3] = to_byte(color[3]);
    if (width != 640 || height != 448) return -1;
    *reinterpret_cast<int*>(base + 17880) = 0;
    if (*reinterpret_cast<unsigned char*>(base + 19536) == 0) sceGsResetGraph(0, 1, 2, 0);
    sceGsSetDefDBuffDc(reinterpret_cast<sceGsDBuffDc*>(norrath_raster_dbuffer), 10, 640, 448, 3, 49, 1);
    const unsigned long long mask = ~0x1ffull;
    unsigned long long packed = static_cast<unsigned long long>(rgba[0]) | 0x3f80000000000000ull |
                                (static_cast<unsigned long long>(rgba[2]) << 16 |
                                 static_cast<unsigned long long>(rgba[1]) << 8) |
                                (static_cast<unsigned long long>(rgba[3]) << 24);
    unsigned& word16 = *reinterpret_cast<unsigned*>(norrath_raster_dbuffer + 16);
    unsigned new16 = (word16 & 0xfffffe00u) | 0x46u;
    unsigned long long v608 = (wide(608) & mask) | 140;
    unsigned long long v96 = wide(96) & mask;
    unsigned long long v224 = wide(224) & mask;
    unsigned long long v464 = (wide(464) & mask) | 70;
    unsigned long long v592 = (wide(592) & mask) | 70;
    unsigned long long v112 = (wide(112) & mask) | 140;
    unsigned long long v240 = (wide(240) & mask) | 140;
    unsigned long long v480 = (wide(480) & mask) | 140;
    wide(608) = v608;
    wide(752) = packed;
    wide(688) = 1;
    word16 = new16;
    wide(96) = v96;
    wide(224) = v224;
    wide(464) = v464;
    wide(592) = v592;
    wide(112) = v112;
    wide(240) = v240;
    wide(480) = v480;
    wide(384) = packed;
    wide(192) = 1;
    wide(320) = 1;
    wide(560) = 1;
    FlushCache(0);
    *reinterpret_cast<float*>(base + 17772) = 16.0f;
    *reinterpret_cast<float*>(base + 17776) = 8388608.0f;
    InitSPRDoubleBuffer__8VIRaster(raster);
    *reinterpret_cast<int*>(base + 17748) = width;
    *reinterpret_cast<int*>(base + 17752) = height;
    return Init__8VIRaster(raster) < 0 ? -1 : 0;
}
}
