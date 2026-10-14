#include "ui_init.cpp"

#include <cstring>
#include <string>

static std::string trace;
static unsigned char* object_bytes;
static int reload_result;
static int load_results[2];
static int load_index;

extern "C" int ReloadWords__4VIUI(VIUI* ui) {
    trace += "R;";
    if (reinterpret_cast<unsigned char*>(ui) != object_bytes) return -100;
    unsigned w[3];
    std::memcpy(w, object_bytes + 72, 12);  // stores happen before the call
    if (w[0] != 0xa1 || w[1] != 0xa2 || w[2] != 0xa3) return -101;
    return reload_result;
}

extern "C" int Load__13VIStringTablePCc(VIStringTable* table, const char* name) {
    trace += "L" + std::to_string(reinterpret_cast<unsigned char*>(table) - object_bytes) + ":" + name + ";";
    return load_results[load_index++];
}

static int run(int reload, int first, int second, int expected_result, const char* expected_trace,
               bool word48_cleared) {
    alignas(4) unsigned char object[96];
    std::memset(object, 0x5a, sizeof object);
    object_bytes = object;
    trace.clear();
    reload_result = reload;
    load_results[0] = first;
    load_results[1] = second;
    load_index = 0;
    int result = Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(
        reinterpret_cast<VIUI*>(object), reinterpret_cast<VIRaster*>(0xa1),
        reinterpret_cast<VISoundDevice*>(0xa2), reinterpret_cast<VIDictionary*>(0xa3));
    if (result != expected_result) return 1;
    if (trace != expected_trace) return 2;
    unsigned w[3];
    std::memcpy(w, object + 72, 12);
    if (w[0] != 0xa1 || w[1] != 0xa2 || w[2] != 0xa3) return 3;
    for (int i = 0; i < 96; ++i) {
        if (i >= 72 && i < 84) continue;
        bool cleared = word48_cleared && i >= 48 && i < 52;
        if (cleared ? object[i] != 0 : object[i] != 0x5a) return 4;
    }
    return 0;
}

int main() {
    if (int e = run(-1, 0, 0, -1, "R;", false)) return e;
    if (int e = run(0, 7, 0, 7, "R;L16:data\\textures.txt;", false)) return 10 + e;
    if (int e = run(0, 0, 5, 5, "R;L16:data\\textures.txt;L32:data\\UISound.txt;", false)) return 20 + e;
    if (int e = run(3, 0, 0, 0, "R;L16:data\\textures.txt;L32:data\\UISound.txt;", true)) return 30 + e;
    return 0;
}
