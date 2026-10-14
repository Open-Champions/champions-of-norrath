// Host check only; not a build input.
#include "channel_count.cpp"
int main() {
    int o[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    if (ChannelCount__C6VIWave(o) != 5) return 1;
    o[5] = -3;
    if (ChannelCount__C6VIWave(o) != -3) return 2;
    return 0;
}
