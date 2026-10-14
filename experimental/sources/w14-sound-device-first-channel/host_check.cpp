// Host check only; not a build input.
#include "first_channel.cpp"
int main() {
    if (FirstChannel__13VISoundDeviceUl(nullptr, 0) != -1) return 1;
    for (int i = 0; i < 46; ++i) {
        unsigned long long m = 1ull << i;
        if (FirstChannel__13VISoundDeviceUl(nullptr, m) != i) return 2;
        if (FirstChannel__13VISoundDeviceUl(nullptr, m | (m << 1) | (1ull << 63)) != i) return 3;
    }
    if (FirstChannel__13VISoundDeviceUl(nullptr, 1ull << 46) != -1) return 4;
    if (FirstChannel__13VISoundDeviceUl(nullptr, ~0ull << 46) != -1) return 5;
    if (FirstChannel__13VISoundDeviceUl(nullptr, ~0ull) != 0) return 6;
    return 0;
}
