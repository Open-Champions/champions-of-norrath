#include "get_time.cpp"

extern "C" {
unsigned long long norrath_vi_timer_system_time __asm__("_7VITimer$SystemTime");
}

int main() {
    norrath_vi_timer_system_time = 0x123456789abcdef0ull;
    if (GetTime__7VITimer() != 0x123456789abcdef0ll) return 1;
    norrath_vi_timer_system_time = 0;
    if (GetTime__7VITimer() != 0) return 2;
    return 0;
}
