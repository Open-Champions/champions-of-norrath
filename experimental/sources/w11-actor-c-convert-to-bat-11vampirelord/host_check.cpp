// Host contract for convertToBat__11VampireLord; not an EE input. Callees are stubs that log.
#define W11_ACTOR_C_HOST 1
#include "convert_to_bat.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
static std::string calls;
static unsigned char* the_object;
static int header_token;
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    calls += std::string("lump(") + lump + "," + name + ");";
    return &header_token;
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags) {
    calls += "anim(" + std::to_string(static_cast<unsigned char*>(state) - the_object) + "," + (header == &header_token ? "H" : "?") + "," + std::to_string(flags) + ");";
}
int main() {
    int failures = 0;
    for (unsigned long long flags : {0ull, 0xffffffffffffffffull, 0x0123456789abcdefull})
        for (unsigned w12 : {0u, 0xfffffffeu, 0x80000000u}) {
            static unsigned char arena[1200];
            std::memset(arena, 0x77, sizeof arena);
            the_object = arena + 16;
            std::memcpy(the_object + 160, &flags, 8);
            std::memcpy(the_object + 12, &w12, 4);
            unsigned char before[1200];
            std::memcpy(before, arena, sizeof arena);
            calls.clear();
            convertToBat__11VampireLord(the_object);
            unsigned long long wantFlags = flags | 0x100000ull;
            unsigned want12 = w12 | 1u;
            std::memcpy(before + 16 + 160, &wantFlags, 8);
            std::memcpy(before + 16 + 12, &want12, 4);
            unsigned one = 1; std::memcpy(before + 16 + 964, &one, 4);
            before[16 + 292] = 0;
            if (std::memcmp(before, arena, sizeof arena) != 0) { std::fprintf(stderr, "memory differs\n"); ++failures; }
            if (calls != "lump(bat.lmp,bat_move.anm);anim(288,H,2049);") { std::fprintf(stderr, "calls: %s\n", calls.c_str()); ++failures; }
        }
    if (!failures) std::puts("convertToBat ok");
    return failures != 0;
}
