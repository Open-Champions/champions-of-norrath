// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "share_sound.cpp"
int main() {
    unsigned char* dev = arena;
    *reinterpret_cast<unsigned*>(dev + 2584 + 4 * 2) = 0x4000;
    *reinterpret_cast<int*>(arena + 0x4000 + 4) = 5;
    if (ShareSound__13VISoundDevicei(dev, 1) != -1) return 1;
    if (ShareSound__13VISoundDevicei(dev, 2) != 0 || *reinterpret_cast<int*>(arena + 0x4004) != 6) return 2;
    if (ShareSound__13VISoundDevicei(dev, 2) != 0 || *reinterpret_cast<int*>(arena + 0x4004) != 7) return 3;
    return 0;
}
