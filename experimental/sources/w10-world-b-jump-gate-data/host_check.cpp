// Host check for the jump-gate storage. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
#include <cstdio>
#include <cstring>
#include "jump_gate_data.cpp"
int main() {
    if (lastDest != 0 || jumpGateState != 0 || jumpGateWho != 0 || jumpGateTime != 0.f ||
        jumpGateDestCount != 0 || jumpGateHurtDest != nullptr) return 1;
    for (int i = 0; i != 480; ++i) if (jumpGateEntries[i] != 0) return 2;
    if (sizeof jumpGateEntries != 480 || sizeof lastDest != 1 || sizeof jumpGateState != 1 ||
        sizeof jumpGateWho != 1 || sizeof jumpGateTime != 4 || sizeof jumpGateDestCount != 1) return 3;
    lastDest = -1; jumpGateWho = -2; jumpGateDestCount = -3;
    if (lastDest >= 0 || jumpGateWho >= 0 || jumpGateDestCount >= 0) return 4;  // signed
    jumpGateState = 200; if (jumpGateState != 200) return 5;                     // unsigned
    std::puts("jump-gate storage check passed");
    return 0;
}
