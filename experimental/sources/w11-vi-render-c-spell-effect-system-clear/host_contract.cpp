#define NORRATH_HOST_CONTRACT 1
#include "clear.cpp"

#include <cstring>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }

static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }

static char* first;
static char* chain[8];    // chain[i] -> next record for record i (by index)
static int log_kind[32];
static unsigned log_arg[32];
static int n;
static unsigned self_seen;

extern "C" char* FirstPlayback__19VISpellEffectSystem(void* s) { log_kind[n] = 1; log_arg[n++] = 0; self_seen = (unsigned)((char*)s - (char*)arena); return first; }
extern "C" char* NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* s, char* p) {
    log_kind[n] = 2; log_arg[n++] = (unsigned)(p - (char*)arena);
    unsigned idx = (unsigned)(p - (char*)arena - 0x2000) / 0x40;
    return chain[idx];
}
extern "C" int Stop__19VISpellEffectSystemi(void* s, int id) { log_kind[n] = 3; log_arg[n++] = id; return 99; }
extern "C" int ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary(void* s, unsigned d) { log_kind[n] = 4; log_arg[n++] = d; return 98; }

int main() {
    const unsigned sys = 0x100, scene = 0x1000;
    std::memset(arena, 0, sizeof arena);
    put(sys, scene);
    put(scene + 19352, 0xd1c7);
    char* rec0 = (char*)arena + 0x2000; char* rec1 = rec0 + 0x40; char* rec2 = rec1 + 0x40;
    put(0x2000 + 28, 11); put(0x2040 + 28, 22); put(0x2080 + 28, 33);
    chain[0] = rec1; chain[1] = rec2; chain[2] = 0;
    void* object = arena + sys;
    // inactive: only 1104 cleared
    put(sys + 1092, 0); put(sys + 1104, 7);
    if (Clear__19VISpellEffectSystem(object) != 0 || n != 0 || get(sys + 1104) != 0 || get(sys + 1092) != 0) return 1;
    // active with three records
    put(sys + 1092, 1); put(sys + 1104, 7); first = rec0; n = 0;
    if (Clear__19VISpellEffectSystem(object) != 0) return 2;
    int expect_kind[] = {1, 2, 3, 2, 3, 2, 3, 4};
    unsigned expect_arg[] = {0, 0x2000, 11, 0x2040, 22, 0x2080, 33, 0xd1c7};
    if (n != 8) return 3;
    for (int i = 0; i < 8; ++i) if (log_kind[i] != expect_kind[i] || log_arg[i] != expect_arg[i]) return 4;
    if (get(sys + 1092) != 0 || get(sys + 1104) != 0) return 5;
    // active with none
    put(sys + 1092, 1); put(sys + 1104, 7); first = 0; n = 0;
    if (Clear__19VISpellEffectSystem(object) != 0 || n != 2 || log_kind[0] != 1 || log_kind[1] != 4) return 6;
    return 0;
}
