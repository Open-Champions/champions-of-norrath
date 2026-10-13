// Host contract for msg_trigger__5Rondoi; not an EE input. The chunk query is a stub that checks
// that the state word is already 2 when called and returns a chosen 64-bit value.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static void* seen_model; static int seen_which, ncalls, state_at_call; static unsigned long long to_return;
static unsigned char* g_self;
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which) {
    seen_model = model; seen_which = which; ++ncalls;
    std::memcpy(&state_at_call, g_self + 244, 4);
    return to_return;
}
#include "msg_trigger.cpp"
alignas(16) static unsigned char arena[0x1000], ref[0x1000];
int main() {
    w10_actor_l_host_base = arena; g_self = arena + 0x100;
    int bad = 0;
    for (int v : {0, 1, -1, 2, 1000}) for (unsigned long long q : {0ull, 0x123456789abcdef0ull, ~0ull}) {
        std::memset(arena, 0x33, sizeof arena);
        unsigned grp = 0x800, model = 0xa00; std::memcpy(arena + 0x100 + 36, &grp, 4); std::memcpy(arena + grp + 12, &model, 4);
        std::memcpy(ref, arena, sizeof arena); ncalls = 0; to_return = q;
        msg_trigger__5Rondoi(arena + 0x100, v);
        if (v != 0) { if (ncalls || std::memcmp(arena, ref, sizeof arena)) bad = 1; continue; }
        const unsigned two = 2, one = 1;
        std::memcpy(ref + 0x100 + 244, &two, 4); std::memcpy(ref + 0x100 + 964, &one, 4); std::memcpy(ref + 0x100 + 552, &q, 8);
        if (ncalls != 1 || seen_model != arena + model || seen_which != 2 || state_at_call != 2) bad = 1;
        if (std::memcmp(arena, ref, sizeof arena)) bad = 1;
    }
    return bad;
}
