// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "trigger.cpp"
#include "../w9-rtti-vtables-entities-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
static int calls = 0, last_arg = 99;
static void* last_self = nullptr;
static void slot(void* self, int arg) { ++calls; last_self = self; last_arg = arg; }
int main() {
    int bad = 0;
    const unsigned flag_values[] = {0u, 0x100000u, 0xffefffffu, 0xffffffffu, 0x80000u};
    const int adjustments[] = {0, 16, -24};
    for (unsigned flags : flag_values) {
        for (int adj : adjustments) {
            unsigned char object[128];
            unsigned char table[256];
            std::memset(object, 0, sizeof object);
            std::memset(table, 0, sizeof table);
            rtti_c::write_word(rtti_c::address(object) + 12u, flags);
            rtti_c::host_install_slot(object, table, 112u, adj, reinterpret_cast<void*>(slot));
            calls = 0; last_arg = 99; last_self = nullptr;
            msg_trigger__10DoorSecreti(object, 5);
            const bool expect_call = (flags & 0x100000u) != 0u;
            const int expect_arg = 0;
            if (calls != (expect_call ? 1 : 0)) { std::printf("call count flags %x\n", flags); ++bad; }
            if (expect_call && (last_self != object + adj || last_arg != expect_arg)) { std::printf("args flags %x adj %d\n", flags, adj); ++bad; }
        }
    }
    std::printf(bad ? "failed\n" : "trigger cases passed\n");
    return bad != 0;
}
