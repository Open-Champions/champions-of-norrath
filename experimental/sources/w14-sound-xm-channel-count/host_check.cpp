// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <cstdint>
#include <cstring>
static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "channel_count.cpp"
int main() {
    unsigned o[12];
    std::memset(o, 0x5a, sizeof o);
    o[9] = 0;
    if (ChannelCount__C4VIXm(o) != 0) return 1;
    o[9] = 0x100;
    short* rec = reinterpret_cast<short*>(arena + 0x100);
    for (int i = 0; i < 8; ++i) rec[i] = static_cast<short>(100 + i);
    if (ChannelCount__C4VIXm(o) != 104) return 2;
    rec[4] = -5;
    if (ChannelCount__C4VIXm(o) != -5) return 3;
    return 0;
}
