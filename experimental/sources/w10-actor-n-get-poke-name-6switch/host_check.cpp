// Host contract for msg_getPokeName__6SwitchPUsi; not an EE input. Includes the source.
#include "get_poke_name.cpp"
#include <cstring>
#include <string>

static std::string calls;
static unsigned short text[4] = {1, 2, 3, 0};
extern "C" unsigned short* textGetString__FPc(const char* key) { calls += std::string("text:") + key + ";"; return text; }
extern "C" void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) { calls += s == text ? "cpy_text;" : "cpy_other;"; d[0] = s[0]; }

int main() {
    unsigned short out[4] = {9, 9, 9, 9};
    int r = msg_getPokeName__6SwitchPUsi(nullptr, out, 123);
    if (r != 1) return 1;
    if (calls != "text:FlipSwitch;cpy_text;") return 2;
    if (out[0] != 1 || out[1] != 9) return 3;
    return 0;
}
