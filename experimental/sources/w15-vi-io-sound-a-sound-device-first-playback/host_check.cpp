// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "first_playback.cpp"
int main() {
    unsigned char* dev = arena;
    unsigned* n0 = reinterpret_cast<unsigned*>(arena + 0x4000);
    *reinterpret_cast<unsigned*>(dev + 3284) = 0x4000;
    n0[0] = 0x4100;
    if (FirstPlayback__13VISoundDevice(dev) != 0x4000) return 1;
    n0[0] = 0;
    if (FirstPlayback__13VISoundDevice(dev) != 0) return 2;
    return 0;
}
