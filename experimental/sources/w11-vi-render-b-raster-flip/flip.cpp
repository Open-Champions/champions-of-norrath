// Observed: Flip__8VIRaster at 0x0110aac0 is 312 bytes. It calls sceGsSyncV(0), then
// sceGsSwapDBuffDc(&_8VIRaster$DBuffer, raster word 17880). Afterwards, with
// A = (doubleword at DBuffer+464 & 511) | 0x51400 and B = (doubleword at DBuffer+96 & 511) | 0x51400
// (the loads follow the swap), and even = the 17880 counter has bit 0 clear, it writes these
// 64-bit values to absolute addresses in this order:
//   0x12000000 = 32547
//   0x12000090 = even ? A : B
//   0x120000a0 = 0x001bf9ff0183227c
//   0x12000070 = even ? A : B
//   0x12000080 = 0x001bf9ff0183327d
//   0x120000d0 = 0
// then calls sceGsSyncPath(0, 0), increments the 17880 counter and returns zero.
// Inferred: the addresses are the EE's GS privileged registers (PMODE at 0x12000000, DISPFB1 at
// 0x12000070, DISPLAY1 at 0x12000080, DISPFB2 at 0x12000090, DISPLAY2 at 0x120000a0, EXTWRITE at
// 0x120000d0), so this selects which half of the double buffer is shown.
// Unknown: the fields of the DBuffer at 96 and 464 (frame addresses of each half), the meaning of
// the two constants beyond their bit patterns, and why PMODE gets 32547.
extern "C" {
struct VIRaster;
struct sceGsDBuffDc;

extern unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");
int sceGsSyncV(int mode);
void sceGsSwapDBuffDc(sceGsDBuffDc* db, int id);
int sceGsSyncPath(int mode, unsigned timeout);

int Flip__8VIRaster(VIRaster* raster) {
    int* counter = reinterpret_cast<int*>(reinterpret_cast<char*>(raster) + 17880);
    sceGsSyncV(0);
    sceGsSwapDBuffDc(reinterpret_cast<sceGsDBuffDc*>(norrath_raster_dbuffer), *counter);
    unsigned long long a = (*reinterpret_cast<unsigned long long*>(norrath_raster_dbuffer + 464) & 511) | 0x51400ull;
    unsigned long long b = (*reinterpret_cast<unsigned long long*>(norrath_raster_dbuffer + 96) & 511) | 0x51400ull;
    bool even = ((~*counter) & 1) != 0;
    volatile unsigned long long* gs = reinterpret_cast<volatile unsigned long long*>(0x12000000u);
    gs[0] = 32547;
    gs[18] = even ? a : b;
    gs[20] = 0x001bf9ff0183227cull;
    gs[14] = even ? a : b;
    gs[16] = 0x001bf9ff0183327dull;
    gs[26] = 0;
    sceGsSyncPath(0, 0);
    *counter += 1;
    return 0;
}
}
