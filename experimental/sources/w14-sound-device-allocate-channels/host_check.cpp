// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <string>
#include <vector>
#include <cstring>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::vector<std::string> log_;
static unsigned records[3] = {0x100, 0x200, 0x300};
static int nrecords;
static unsigned long long* device_mask;
extern "C" unsigned FirstPlayback__13VISoundDevice(void*) { return nrecords ? records[0] : 0; }
extern "C" unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void*, unsigned p) {
    for (int i = 0; i < nrecords; ++i) if (records[i] == p) return i + 1 < nrecords ? records[i + 1] : 0;
    return 0;
}
extern "C" int ChannelCount__13VISoundDeviceUl(void*, unsigned long long m) {
    int n = 0;
    for (int i = 0; i < 46; ++i) n += static_cast<int>((m >> i) & 1);
    return n;
}
extern "C" void Stop__13VISoundDevicei(void*, int v) {
    log_.push_back("stop" + std::to_string(v));
    for (int i = 0; i < nrecords; ++i) {
        unsigned* r = reinterpret_cast<unsigned*>(arena + records[i]);
        if (static_cast<int>(r[32 / 4]) == v) {
            unsigned long long m = r[16 / 4] | (static_cast<unsigned long long>(r[20 / 4]) << 32);
            *device_mask &= ~m;
        }
    }
}
#include "allocate_channels.cpp"
static void rec(int i, unsigned long long mask, int priority, int stop) {
    unsigned* r = reinterpret_cast<unsigned*>(arena + records[i]);
    r[16 / 4] = static_cast<unsigned>(mask);
    r[20 / 4] = static_cast<unsigned>(mask >> 32);
    r[24 / 4] = static_cast<unsigned>(priority);
    r[32 / 4] = static_cast<unsigned>(stop);
}
int main() {
    alignas(8) unsigned char dev[64];
    std::memset(dev, 0, sizeof dev);
    unsigned long long* mask = reinterpret_cast<unsigned long long*>(dev);
    device_mask = mask;
    nrecords = 0;
    // plenty free: lowest clear bits
    *mask = 0;
    if (AllocateChannels__13VISoundDeviceii(dev, 3, 5) != 7 || *mask != 7) return 1;
    *mask = 0x5;
    if (AllocateChannels__13VISoundDeviceii(dev, 2, 5) != 0xa || *mask != 0xf) return 2;
    *mask = 0x1;
    if (AllocateChannels__13VISoundDeviceii(dev, 1, 5) != 2 || *mask != 3) return 3;
    *mask = 0;
    if (AllocateChannels__13VISoundDeviceii(dev, 1, 5) != 1 || *mask != 1) return 4;
    // count zero takes every free bit
    *mask = 0x6;
    unsigned long long all = (1ull << 46) - 1;
    if (AllocateChannels__13VISoundDeviceii(dev, 0, 5) != (all & ~6ull) || *mask != all) return 5;
    // bits above 45 are not counted as used
    *mask = (1ull << 50);
    if (AllocateChannels__13VISoundDeviceii(dev, 1, 5) != 1 || *mask != ((1ull << 50) | 1)) return 6;
    // full pool, request cannot be satisfied: nothing changes
    nrecords = 2;
    rec(0, 0x3, 9, 100); rec(1, 0xc, 9, 101);
    *mask = all;
    log_.clear();
    if (AllocateChannels__13VISoundDeviceii(dev, 8, 4) != 0 || *mask != all || !log_.empty()) return 7;
    // evict records with priority <= requested: needs 3, record 0 gives 2, record 1 gives 2
    rec(0, 0x3, 4, 100); rec(1, 0xc, 3, 101);
    log_.clear();
    unsigned long long r = AllocateChannels__13VISoundDeviceii(dev, 3, 4);
    if (log_.size() != 2 || log_[0] != "stop100" || log_[1] != "stop101") return 8;
    if (r != 0x7) return 9;
    if (*mask != (all & ~8ull)) return 10;
    // eviction stops once enough is free; free = 0 here
    rec(0, 0x3, 4, 100); rec(1, 0xc, 3, 101);
    *mask = all;
    log_.clear();
    r = AllocateChannels__13VISoundDeviceii(dev, 2, 4);
    if (log_.size() != 1 || log_[0] != "stop100" || r != 3 || *mask != all) return 11;
    // priority skip in the first pass makes the request fail when only a skipped record could help
    rec(0, 0x3, 9, 100); rec(1, 0xc, 3, 101);
    *mask = all;
    log_.clear();
    if (AllocateChannels__13VISoundDeviceii(dev, 3, 4) != 0 || !log_.empty()) return 12;
    // upper channel bits of a record (above 31) are counted
    nrecords = 1;
    rec(0, (3ull << 40), 4, 77);
    *mask = all;
    log_.clear();
    if (AllocateChannels__13VISoundDeviceii(dev, 2, 4) != (3ull << 40) || log_.size() != 1) return 13;
    return 0;
}
