// Host check for coreHeadsetResetTimer. Not an EE plan input.
#include "core_headset_reset_timer.cpp"

int main() {
    if (sizeof(coreHeadsetResetTimer) != 4 || coreHeadsetResetTimer != 0) return 1;
    return 0;
}
