#include "delay_thread_name.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_delay_thread_name_view == 12, "DelayThread and NUL");
    if (std::strcmp(reinterpret_cast<const char*>(norrath_sound_delay_thread_name_view), "DelayThread") != 0)
        return 1;
    std::puts("delay thread name passed");
    return 0;
}
