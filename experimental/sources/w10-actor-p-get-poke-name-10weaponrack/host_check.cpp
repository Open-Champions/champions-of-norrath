// Host contract for msg_getPokeName__10WeaponRackPUsi; not an EE input. Includes the source.
#include "get_poke_name.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string calls;
static unsigned short result[4] = {1, 2, 3, 0};
extern "C" const unsigned short* textGetString__FPc(const char* key) { calls += std::string("text:") + key + ";"; return result; }
extern "C" void ustrcpy__FPUsT0(unsigned short* d, const unsigned short* s) {
    calls += s == result ? "copy:result;" : "copy:other;";
    for (int i = 0; i < 4; ++i) d[i] = s[i];
}
int main() {
    unsigned short buffer[8] = {9, 9, 9, 9, 9, 9, 9, 9};
    int r = msg_getPokeName__10WeaponRackPUsi(nullptr, buffer);
    if (r != 1 || calls != "text:SearchWeaponRack;copy:result;") { std::puts(calls.c_str()); return 1; }
    for (int i = 0; i < 4; ++i) if (buffer[i] != result[i]) return 2;
    return buffer[4] != 9;
}
