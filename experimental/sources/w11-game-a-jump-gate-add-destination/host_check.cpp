// Host contract for jumpGateAddDestination; not an EE input.
#define strcmp norrath_hc_strcmp
#define strcpy norrath_hc_strcpy
#include "jump_gate_add_destination.cpp"
#undef strcmp
#undef strcpy
#include <cstdio>
#include <cstring>
signed char jumpGateDestCount;
char jumpGateEntries[480];
static const char* current = "alpha";
static int copies;
void gameGetFullLevelName__FPc(char* buffer) { std::strcpy(buffer, current); }
extern "C" int norrath_hc_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
extern "C" char* norrath_hc_strcpy(char* d, const char* s) { ++copies; return std::strcpy(d, s); }
int main() {
    int bad = 0;
    jumpGateAddDestination__Fv();
    if (jumpGateDestCount != 1 || std::strcmp(jumpGateEntries, "alpha") != 0) { std::puts("fail: first add"); bad = 1; }
    jumpGateAddDestination__Fv();
    if (jumpGateDestCount != 1 || copies != 1) { std::puts("fail: duplicate added"); bad = 1; }
    current = "beta";
    jumpGateAddDestination__Fv();
    if (jumpGateDestCount != 2 || std::strcmp(jumpGateEntries + 32, "beta") != 0 || std::strcmp(jumpGateEntries, "alpha") != 0) { std::puts("fail: second add"); bad = 1; }
    current = "alpha";
    jumpGateAddDestination__Fv();
    current = "beta";
    jumpGateAddDestination__Fv();
    if (jumpGateDestCount != 2 || copies != 2) { std::puts("fail: matches not found"); bad = 1; }
    // only the first count entries are searched
    current = "gamma";
    std::strcpy(jumpGateEntries + 64, "gamma");
    jumpGateAddDestination__Fv();
    if (jumpGateDestCount != 3 || copies != 3) { std::puts("fail: stale entry searched"); bad = 1; }
    if (!bad) std::puts("jumpGateAddDestination ok");
    return bad;
}
