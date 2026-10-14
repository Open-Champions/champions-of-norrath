// Observed Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary at
// 0x0102f2c8, 120 bytes. The three arguments are stored at offsets 72, 76 and
// 80 of the object. ReloadWords__4VIUI(object) then runs; a negative result
// returns -1 at once. Otherwise VIStringTable::Load runs on the table at
// offset 16 with "data\textures.txt"; a nonzero result is returned. Otherwise
// Load runs on the table at offset 32 with "data\UISound.txt"; when that
// returns zero the word at offset 48 is cleared. The last Load result is
// returned. Which Load result means failure is not established here.
extern "C" {
struct VIUI;
struct VIRaster;
struct VISoundDevice;
struct VIDictionary;
struct VIStringTable;

int ReloadWords__4VIUI(VIUI* ui);
int Load__13VIStringTablePCc(VIStringTable* table, const char* name);

int Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(VIUI* ui, VIRaster* raster,
                                                         VISoundDevice* sound,
                                                         VIDictionary* dictionary) {
    char* base = reinterpret_cast<char*>(ui);
    *reinterpret_cast<unsigned*>(base + 72) = static_cast<unsigned>(reinterpret_cast<unsigned long>(raster));
    *reinterpret_cast<unsigned*>(base + 76) = static_cast<unsigned>(reinterpret_cast<unsigned long>(sound));
    *reinterpret_cast<unsigned*>(base + 80) = static_cast<unsigned>(reinterpret_cast<unsigned long>(dictionary));
    if (ReloadWords__4VIUI(ui) < 0) return -1;
    int result = Load__13VIStringTablePCc(reinterpret_cast<VIStringTable*>(base + 16),
                                          "data\\textures.txt");
    if (result != 0) return result;
    result = Load__13VIStringTablePCc(reinterpret_cast<VIStringTable*>(base + 32),
                                      "data\\UISound.txt");
    if (result == 0) *reinterpret_cast<unsigned*>(base + 48) = 0;
    return result;
}
}
