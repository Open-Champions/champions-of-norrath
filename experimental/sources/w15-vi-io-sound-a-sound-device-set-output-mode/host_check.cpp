// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include <vector>
#include <cstring>
static std::vector<unsigned long long> log_;
extern "C" void SetPan__13VISoundDeviceif(unsigned char*, int handle, float pan) {
    unsigned bits;
    std::memcpy(&bits, &pan, 4);
    log_.push_back((static_cast<unsigned long long>(handle) << 32) | bits);
}
#include "set_output_mode.cpp"
extern "C" unsigned FirstPlayback__13VISoundDevice(unsigned char* device) {
    unsigned n = *reinterpret_cast<unsigned*>(device + 3284);
    return *reinterpret_cast<unsigned*>(arena + n) ? n : 0;
}
extern "C" unsigned NextPlayback__13VISoundDeviceP10VIPlayback(unsigned char*, unsigned p) {
    unsigned n = *reinterpret_cast<unsigned*>(arena + p);
    return *reinterpret_cast<unsigned*>(arena + n) ? n : 0;
}
int main() {
    unsigned char* dev = arena;
    *reinterpret_cast<unsigned*>(dev + 3284) = 0x4000;
    *reinterpret_cast<unsigned*>(arena + 0x4000) = 0x4100;
    *reinterpret_cast<unsigned*>(arena + 0x4100) = 0x4200;
    *reinterpret_cast<unsigned*>(arena + 0x4200) = 0;
    *reinterpret_cast<unsigned*>(arena + 0x4020) = 21;
    *reinterpret_cast<unsigned*>(arena + 0x4120) = 22;
    *reinterpret_cast<float*>(arena + 0x402c) = 0.5f;
    *reinterpret_cast<float*>(arena + 0x412c) = -1.0f;
    SetOutputMode__13VISoundDevicei(dev, 3);
    if (*reinterpret_cast<int*>(dev + 3432) != 3) return 1;
    if (log_.size() != 2) return 2;
    if (log_[0] != ((21ull << 32) | 0x3f000000u)) return 3;
    if (log_[1] != ((22ull << 32) | 0xbf800000u)) return 4;
    *reinterpret_cast<unsigned*>(arena + 0x4000) = 0;
    log_.clear();
    SetOutputMode__13VISoundDevicei(dev, 9);
    if (*reinterpret_cast<int*>(dev + 3432) != 9 || !log_.empty()) return 5;
    return 0;
}
