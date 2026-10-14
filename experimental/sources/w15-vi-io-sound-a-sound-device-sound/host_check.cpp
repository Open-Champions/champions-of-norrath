// Host check only; not a build input.
#include "sound.cpp"
int main() {
    alignas(8) unsigned char dev[2700] = {};
    *reinterpret_cast<unsigned*>(dev + 2584) = 0x11;
    *reinterpret_cast<unsigned*>(dev + 2584 + 4 * 5) = 0x55;
    if (Sound__13VISoundDevicei(dev, -1) != 0) return 1;
    if (Sound__13VISoundDevicei(dev, 0) != 0x11) return 2;
    if (Sound__13VISoundDevicei(dev, 5) != 0x55) return 3;
    if (Sound__13VISoundDevicei(dev, 1) != 0) return 4;
    return 0;
}
