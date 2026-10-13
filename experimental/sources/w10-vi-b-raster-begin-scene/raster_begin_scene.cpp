// Observed BeginScene__8VIRaster at 0x01112d98, 196 bytes. If the byte at
// offset 19536 of the raster is nonzero it is cleared and the display is
// restarted: sceGsResetGraph(0, 1, 2, 0), then three times
// sceGsSyncV(0); sceGsSwapDBuffDc(&_8VIRaster$DBuffer, n) with n read from the
// counter at offset 17880, which is incremented after each swap, each
// followed by a 64-bit store of 32544 to the absolute address 0x12000000.
// In every case the word at 0x011eda84 (inside the file-local SurfCache
// table, 0x1c bytes in) is then cleared and SetGSScissoring__8VIRaster(raster)
// runs. The result is zero. What the 0x12000000 store programs is not
// established here.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store64(unsigned address, unsigned long long value);
#define NORRATH_STORE64(address, value) norrath_host_store64((address), (value))
#else
#define NORRATH_STORE64(address, value)                                              \
    do {                                                                             \
        *reinterpret_cast<volatile unsigned long long*>(address) = (value);         \
    } while (0)
#endif

extern "C" {
struct VIRaster;
struct sceGsDBuffDc;

extern unsigned char norrath_raster_dbuffer[0x330] __asm__("_8VIRaster$DBuffer");
extern unsigned norrath_surf_cache[] __asm__("SurfCache");

int sceGsResetGraph(int mode, int interlace, int output, int ffmode);
int sceGsSyncV(int mode);
void sceGsSwapDBuffDc(sceGsDBuffDc* db, int id);
void SetGSScissoring__8VIRaster(VIRaster* raster);

int BeginScene__8VIRaster(VIRaster* raster) {
    char* base = reinterpret_cast<char*>(raster);
    unsigned char* flag = reinterpret_cast<unsigned char*>(base + 19536);
    if (*flag != 0) {
        *flag = 0;
        sceGsResetGraph(0, 1, 2, 0);
        int* counter = reinterpret_cast<int*>(base + 17880);
        for (int i = 0; i < 3; ++i) {
            sceGsSyncV(0);
            int id = *counter;
            sceGsSwapDBuffDc(reinterpret_cast<sceGsDBuffDc*>(norrath_raster_dbuffer), id);
            *counter = id + 1;
            NORRATH_STORE64(0x12000000u, 32544ull);
        }
    }
    norrath_surf_cache[7] = 0;
    SetGSScissoring__8VIRaster(raster);
    return 0;
}
}
