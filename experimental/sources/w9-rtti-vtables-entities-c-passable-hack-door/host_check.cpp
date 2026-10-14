// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "passable.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int calls = 0, seen_flag = 99;
static void* seen_self = nullptr;
static unsigned flags_at_call = 0;
extern "C" void setPassableState__8AutoPropb(void* self, int passable) {
    ++calls; seen_self = self; seen_flag = passable;
    flags_at_call = rtti_c::read_word(rtti_c::address(self) + 12u);
}
int main() {
    int bad = 0;
    const unsigned start[] = {0u, 1u, 0x80000000u, 0x12345670u};
    for (unsigned s : start) {
        for (int flag : {0, 1, 5}) {
            unsigned char object[32];
            std::memset(object, 0xaa, sizeof object);
            rtti_c::write_word(rtti_c::address(object) + 12u, s);
            calls = 0;
            setPassableState__8HackDoorb(object, flag);
            if (calls != 1 || seen_self != object || seen_flag != flag) { std::printf("call\n"); ++bad; }
            if (flags_at_call != s) { std::printf("flag set before call\n"); ++bad; }
            if (rtti_c::read_word(rtti_c::address(object) + 12u) != (s | 1u)) { std::printf("flags %x\n", s); ++bad; }
            if (object[11] != 0xaa || object[16] != 0xaa) { std::printf("neighbours\n"); ++bad; }
        }
    }
    std::printf(bad ? "failed\n" : "setPassableState__8HackDoorb passed\n");
    return bad != 0;
}
