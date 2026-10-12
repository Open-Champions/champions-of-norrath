// Host check for sync_state.cpp; not an EE plan input.
#include "sync_state.cpp"

int main() {
    if (syncState != 0) return 1;
    syncState = 5;
    if (syncState != 5) return 2;
    return 0;
}
