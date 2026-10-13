// Host check for ee_timer_words.cpp: observed widths and the zero start state.
#include <cstdio>
#include "ee_timer_words.cpp"

static_assert(sizeof(rollover_counter) == 8, "observed 8-byte counter span");
static_assert(sizeof(timer_handle) == 4, "observed 4-byte handle word");

int main() {
    int bad = 0;
    for (unsigned char b : rollover_counter) if (b != 0) bad = 1;
    if (timer_handle != 0) bad = 1;
    std::puts(bad ? "FAIL" : "PASS");
    return bad;
}
