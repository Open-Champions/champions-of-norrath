#include "reset_tick_pulse.cpp"

static long long clock_value;
extern "C" long long GetTime__7VITimer() { return clock_value; }

int main() {
    alignas(16) unsigned char window[400] = {};
    clock_value = 0x123456789abcdef0ll;
    ResetTickPulse__10VIUIWindow(window);
    if (*reinterpret_cast<long long*>(window + 352) != clock_value) return 1;
    for (int i = 0; i < 400; ++i)
        if ((i < 352 || i >= 360) && window[i] != 0) return 2;
    return 0;
}
