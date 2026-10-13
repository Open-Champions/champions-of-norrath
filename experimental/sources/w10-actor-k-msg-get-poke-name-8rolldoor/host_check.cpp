// Host contract for msg_getPokeName__8RollDoorPUsi; not an EE input.
#include <cstring>
#include <string>
static std::string calls; static unsigned short text[4] = {7, 8, 9, 0};
extern "C" unsigned short* textGetString__FPc(const char* k) { calls += std::string("text(") + k + ");"; return text; }
extern "C" void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from) { calls += "copy;"; std::memcpy(to, from, 8); }
#include "msg_get_poke_name.cpp"
int main() {
    unsigned short out[6]; std::memset(out, 0xff, sizeof out);
    const int r = msg_getPokeName__8RollDoorPUsi(nullptr, out);
    return !(r == 1 && calls == "text(OpenDoor);copy;" && out[0] == 7 && out[2] == 9 && out[3] == 0 && out[4] == 0xffff);
}
