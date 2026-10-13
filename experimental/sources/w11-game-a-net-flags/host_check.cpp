// Host contract for the net flag storage; not an EE input.
#include "net_flags.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof netTrueStep == 1 && sizeof netPrediction == 1 && sizeof netPredictionFastForward == 1 && sizeof netLogDisable == 1, "byte flags");
    int bad = 0;
    if (netTrueStep != 0 || netPrediction != 0 || netPredictionFastForward != 0 || netLogDisable != 0) { std::puts("fail: initial values"); bad = 1; }
    netTrueStep = 1; netPrediction = -1;
    if (&netTrueStep == &netPrediction || &netPredictionFastForward == &netLogDisable || netPredictionFastForward != 0 || netLogDisable != 0) { std::puts("fail: separate storage"); bad = 1; }
    if (!bad) std::puts("net flags ok");
    return bad;
}
