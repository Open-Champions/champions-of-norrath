// Host check only; not a build input.
static unsigned result;
static int seen;
extern "C" unsigned Playback__13VISoundDevicei(unsigned char*, int handle) { seen = handle; return result; }
#include "is_playing.cpp"
int main() {
    result = 0;
    if (IsPlaying__13VISoundDevicei(0, 9) != 0 || seen != 9) return 1;
    result = 0x1234;
    if (IsPlaying__13VISoundDevicei(0, 4) != 1 || seen != 4) return 2;
    result = 0x80000000u;
    if (IsPlaying__13VISoundDevicei(0, 4) != 1) return 3;
    return 0;
}
