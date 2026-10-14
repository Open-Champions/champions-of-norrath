// Host check for netStartLevel. Not an EE plan input.
#include "net_start_level.cpp"

extern "C" { int localPredictedPlayer = 7; }

int main() {
    netStartLevel__Fv();
    if (localPredictedPlayer != 0) return 1;
    localPredictedPlayer = -3;
    netStartLevel__Fv();
    return localPredictedPlayer == 0 ? 0 : 2;
}
