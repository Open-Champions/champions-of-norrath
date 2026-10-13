// Host contract for JumpGate msg_poke; not an EE input.
#include "msg_poke.cpp"
#include <cstdio>
signed char netNetworkMode;
static int mutexResult, mutexCalls, inits; static int seenValue; static char* seenName; static char seenFlag;
int DLG_CheckMutex__Fv() { ++mutexCalls; return mutexResult; }
void jumpGateInit__FiPcc(int v, char* n, char f) { ++inits; seenValue = v; seenName = n; seenFlag = f; }
int main() {
    int bad = 0, dummy;
    const int mutexes[] = {0, 0, 1, 1, 7, -1};
    const int modes[] = {0, 3, 0, 1, 0, -2};
    for (int k = 0; k < 6; ++k) {
        mutexResult = mutexes[k]; netNetworkMode = static_cast<signed char>(modes[k]); mutexCalls = inits = 0; seenName = reinterpret_cast<char*>(1); seenFlag = 9;
        msg_poke__8JumpGatei(&dummy, 40 + k);
        bool expectInit = mutexes[k] == 0 || modes[k] != 0;
        if (mutexCalls != 1) { std::puts("fail: mutex call count"); bad = 1; }
        if ((inits == 1) != expectInit) { std::printf("fail: init decision case %d\n", k); bad = 1; }
        if (expectInit && (seenValue != 40 + k || seenName != nullptr || seenFlag != 0)) { std::puts("fail: init arguments"); bad = 1; }
    }
    if (!bad) std::puts("jumpgate msg_poke ok");
    return bad;
}
