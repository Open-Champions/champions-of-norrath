// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <vector>
#include <cstring>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
struct Call { int voice; float volume; };
static std::vector<Call> calls;
static std::vector<unsigned> order;
static unsigned records[3] = {0x100, 0x200, 0x300};
extern "C" unsigned FirstPlayback__13VISoundDevice(void*) { order.push_back(1); return records[0]; }
extern "C" unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void*, unsigned p) {
    order.push_back(2);
    for (int i = 0; i < 3; ++i) if (records[i] == p) return i == 2 ? 0 : records[i + 1];
    return 0;
}
extern "C" void SetVolume__13VISoundDeviceif(void*, int voice, float volume) { calls.push_back({voice, volume}); }
#include "set_class_volume.cpp"
int main() {
    alignas(8) unsigned char dev[4096];
    std::memset(dev, 0, sizeof dev);
    unsigned* r = reinterpret_cast<unsigned*>(arena + 0x100);
    r[52 / 4] = 2; r[32 / 4] = 11;
    float f = 0.5f; std::memcpy(&r[40 / 4], &f, 4);
    r = reinterpret_cast<unsigned*>(arena + 0x200);
    r[52 / 4] = 3; r[32 / 4] = 12;
    f = 0.25f; std::memcpy(&r[40 / 4], &f, 4);
    r = reinterpret_cast<unsigned*>(arena + 0x300);
    r[52 / 4] = 2; r[32 / 4] = 13;
    f = 0.75f; std::memcpy(&r[40 / 4], &f, 4);
    SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(dev, 2, 0.125f);
    float table;
    std::memcpy(&table, dev + 3424 + 8, 4);
    if (table != 0.125f) return 1;
    if (calls.size() != 2 || calls[0].voice != 11 || calls[0].volume != 0.5f || calls[1].voice != 13 || calls[1].volume != 0.75f) return 2;
    if (order.size() != 4) return 3;
    for (int i = 0; i < 4096; ++i) if (dev[i] && (i < 3432 || i >= 3436)) return 4;
    calls.clear();
    SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(dev, 5, 1.0f);
    if (!calls.empty()) return 5;
    return 0;
}
