// Host check only; not a build input.
#include "next_channel.cpp"
int main() {
    unsigned long long m = (1ull << 3) | (1ull << 10) | (1ull << 45) | (1ull << 46) | (1ull << 63);
    if (NextChannel__13VISoundDeviceUli(nullptr, m, -1) != 3) return 1;
    if (NextChannel__13VISoundDeviceUli(nullptr, m, 3) != 10) return 2;
    if (NextChannel__13VISoundDeviceUli(nullptr, m, 10) != 45) return 3;
    if (NextChannel__13VISoundDeviceUli(nullptr, m, 45) != -1) return 4;
    if (NextChannel__13VISoundDeviceUli(nullptr, m, 46) != -1) return 5;
    if (NextChannel__13VISoundDeviceUli(nullptr, m, 100) != -1) return 6;
    if (NextChannel__13VISoundDeviceUli(nullptr, 0, 0) != -1) return 7;
    if (NextChannel__13VISoundDeviceUli(nullptr, ~0ull, 4) != 5) return 8;
    return 0;
}
