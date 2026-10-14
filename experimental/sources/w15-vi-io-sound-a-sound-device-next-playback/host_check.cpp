// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "next_playback.cpp"
int main() {
    unsigned* a = reinterpret_cast<unsigned*>(arena + 0x4000);
    unsigned* b = reinterpret_cast<unsigned*>(arena + 0x4100);
    unsigned* c = reinterpret_cast<unsigned*>(arena + 0x4200);
    a[0] = 0x4100; b[0] = 0x4200; c[0] = 0;
    if (NextPlayback__13VISoundDeviceP10VIPlayback(arena, 0x4000) != 0x4100) return 1;
    if (NextPlayback__13VISoundDeviceP10VIPlayback(arena, 0x4100) != 0) return 2;
    return 0;
}
