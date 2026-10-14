// Host check for sync_rand_state.cpp; not an EE plan input.
#include "sync_rand_state.cpp"

int main() {
    if (syncRandState != 0) return 1;
    syncRandState = 0x12345678u;
    if (syncRandState != 0x12345678u) return 2;
    return 0;
}
