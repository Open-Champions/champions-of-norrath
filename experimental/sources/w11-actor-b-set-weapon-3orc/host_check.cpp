// Host contract for setWeapon__3Orc; not an EE input. Includes the source.
#define W11_ACTOR_B_HOST 1
#include "set_weapon.cpp"
#include <cstring>
#include <initializer_list>
static unsigned char arena[0x1000];
unsigned char* w11_actor_b_host_base = arena;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static unsigned rolls[2];
static int calls = 0;
extern "C" unsigned eRand__10GameObject(void*) { return rolls[calls++ & 1]; }
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    for (unsigned type = 0; type < 9; ++type)
        for (unsigned r = 0; r < 128; ++r)
            for (unsigned r2 : {0u, 1u, 2u, 3u, 4u, 0xfffffffeu, 0xffffffffu}) {
                std::memset(arena, 0x77, sizeof arena);
                W(1116) = type; rolls[0] = r; rolls[1] = r2; calls = 0;
                unsigned char before[sizeof arena]; std::memcpy(before, arena, sizeof arena);
                setWeapon__3Orc(arena);
                int want972 = -1, want1124 = -1, wantCalls = 1;
                if (type == 0) want972 = (r & 8) ? 1 : (r & 0x16) ? 4 : 3;
                if (type == 4) { want972 = 2; want1124 = 1; }
                if (type == 1) want972 = (r & 0x16) ? 5 : (r & 0x32) ? 7 : 14;
                if (type == 2 || type == 3) { want972 = 14 + r2 % 3; wantCalls = 2; }
                if (calls != wantCalls) return 1;
                if (want972 < 0 ? W(972) != 0x77777777u : W(972) != static_cast<unsigned>(want972)) return 2;
                if (want1124 < 0 ? W(1124) != 0x77777777u : W(1124) != 1u) return 3;
                for (unsigned i = 0; i < sizeof arena; ++i)
                    if (!((i >= 972 && i < 976) || (i >= 1124 && i < 1128)) && before[i] != arena[i]) return 4;
            }
    return 0;
}
