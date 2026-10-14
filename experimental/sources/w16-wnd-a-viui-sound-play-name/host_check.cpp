// Host check only; not a build input. Also compared with the original in a private interpreter (96 paired
// scenarios, all 30 words covered). The UI object lives in the arena (device and dictionary words are guest
// addresses). Callees log; the expected log follows the description in the source comment.
#define NORRATH_HOST_ARENA 1
#include "sound_play_name.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

static const unsigned ui_g = hw::BASE + 0x100, device_g = hw::BASE + 0x2000, dict_g = hw::BASE + 0x3000;
static std::string trace;
static int find_result, play_result;
static std::string N(long v) { return std::to_string(v); }
static std::string fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return N(v); }
extern "C" unsigned VIHashResourceID(const char* name) { trace += std::string("H:") + name + ";"; return 0x1000u + static_cast<unsigned>(std::strlen(name)); }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* type, int* handle) {
    trace += "F" + N(hw::guest(d)) + "," + N(id) + ";";
    *type = 3;
    *handle = 9000;
    return find_result;
}
extern "C" int Play__13VISoundDeviceii12VISOUNDCLASS(void* d, int handle, int level, int cls) {
    trace += "P" + N(hw::guest(d)) + "," + N(handle) + "," + N(level) + "," + N(cls) + ";";
    return play_result;
}
extern "C" void SetVolume__13VISoundDeviceif(void* d, int voice, float volume) { trace += "V" + N(hw::guest(d)) + "," + N(voice) + "," + fbits(volume) + ";"; }

int main() {
    bool ok = true;
    for (int found : {0, 1, -1, 7}) for (int voice : {-7, -1, 0, 3, 127}) for (float volume : {0.0f, 0.35f, 1.0f, -2.5f}) for (const char* name : {"a", "ClickSound"}) {
        hw::clear();
        hw::w(ui_g + 76) = device_g;
        hw::w(ui_g + 80) = dict_g;
        find_result = found;
        play_result = voice;
        trace.clear();
        const int result = SoundPlay__4VIUIPCcf(hw::at(ui_g), name, volume);
        std::string want = std::string("H:") + name + ";F" + N(dict_g) + "," + N(0x1000u + std::strlen(name)) + ";";
        if (found != 0) {
            want += "P" + N(device_g) + ",9000,127,0;";
            if (voice >= 0) want += "V" + N(device_g) + "," + N(voice) + "," + fbits(volume) + ";";
        }
        if (result != 0 || trace != want) { ok = false; std::printf("MISMATCH found=%d voice=%d got '%s' want '%s'\n", found, voice, trace.c_str(), want.c_str()); break; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
