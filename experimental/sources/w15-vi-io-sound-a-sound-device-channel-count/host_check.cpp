// Host check only; not a build input.
#include "channel_count.cpp"
int main() {
    if (ChannelCount__13VISoundDeviceUl(0, 0) != 0) return 1;
    if (ChannelCount__13VISoundDeviceUl(0, (1ull << 46) - 1) != 46) return 2;
    if (ChannelCount__13VISoundDeviceUl(0, ~0ull) != 46) return 3;
    if (ChannelCount__13VISoundDeviceUl(0, 1ull << 46) != 0) return 4;
    if (ChannelCount__13VISoundDeviceUl(0, 1ull << 45) != 1) return 5;
    if (ChannelCount__13VISoundDeviceUl(0, 0x5555555555555555ull) != 23) return 6;
    return 0;
}
