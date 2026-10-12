// Host contract for endTimer.1059. Not an EE plan input.
#include "end_timer_1059.cpp"

int main() {
    if (endTimerPause != 0) return 1;
    if (sizeof endTimerPause != 4u) return 2;
    endTimerPause = endTimerPause + 1;
    if (endTimerPause != 0 + 1) return 3;
    return 0;
}
