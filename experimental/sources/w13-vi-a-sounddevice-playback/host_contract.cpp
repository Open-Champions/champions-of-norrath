#define NORRATH_HOST_ARENA 1
#include "playback.cpp"
#include <string.h>

static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

int main() {
    unsigned char* dev = arena;            // device at arena offset 0
    unsigned char* a = arena + 0x1000;     // two playback objects
    unsigned char* b = arena + 0x1100;
    memset(arena, 0, sizeof arena);
    if (Playback__13VISoundDevicei(dev, 7) != 0) return 1;
    *(int*)(a + 32) = 300;                 // 300 & 0xff = 44, 44 % 46 = 44
    *(unsigned*)(dev + 3100 + 4 * 44) = 0x1000;
    if (Playback__13VISoundDevicei(dev, 300) != a) return 2;
    if (Playback__13VISoundDevicei(dev, 44) != 0) return 3;   // same bucket, id differs
    *(int*)(b + 32) = 46;                  // 46 % 46 = 0
    *(unsigned*)(dev + 3100) = 0x1100;
    if (Playback__13VISoundDevicei(dev, 46) != b) return 4;
    if (Playback__13VISoundDevicei(dev, 46 + 256) != 0) return 5;  // id differs in the high bits
    *(int*)(b + 32) = 46 + 256;
    if (Playback__13VISoundDevicei(dev, 46 + 256) != b) return 6;
    return 0;
}
