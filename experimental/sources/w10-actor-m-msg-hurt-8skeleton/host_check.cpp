// Host contract for msg_hurt__8Skeleton...; not an EE input. The slot-0 function, __tf8Skeleton, the
// type comparison and the base hurt are stubs; the slot-0 stub checks it receives the hurt object.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_m_host_base;
static void *slot_arg, *base_self, *base_src; static int nslot, nbase, eq_ret, base_flag; static float base_amt;
static const void *eq_a, *eq_b; static unsigned slot_fn; static int neq;
unsigned w10_actor_m_host_call(unsigned fn, void* a0, int, unsigned long, unsigned long) { ++nslot; slot_fn = fn; slot_arg = a0; return 0x7777u; }
static unsigned char tinfo[16];
extern "C" void* __tf8Skeleton() { return tinfo; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) { ++neq; eq_a = a; eq_b = b; return eq_ret; }
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* s, float amt, void* src, int f) { ++nbase; base_self = s; base_amt = amt; base_src = src; base_flag = f; }
#include "msg_hurt.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x2000];
    w10_actor_m_host_base = arena;
    for (int ret : {0, 1, 2, 3, -1}) for (int hasSrc : {1, 0}) for (int flag : {0, 5, -3}) for (float amt : {1.5f, -4.0f}) {
        std::memset(arena, 0, sizeof arena);
        const unsigned SELF = 0x100, SRC = 0x800, VT = 0x1000;
        unsigned vt = VT; std::memcpy(arena + SRC + 72, &vt, 4);
        short adj = 4; std::memcpy(arena + VT, &adj, 2);
        unsigned fn = 0xabcd0000u; std::memcpy(arena + VT + 4, &fn, 4);
        nslot = nbase = neq = 0; eq_ret = ret;
        msg_hurt__8SkeletonfP10GameObjecti(arena + SELF, amt, hasSrc ? arena + SRC : nullptr, flag);
        if (!hasSrc) { if (nslot || neq || nbase) bad = 1; continue; }
        if (nslot != 1 || slot_fn != fn || slot_arg != arena + SELF) bad = 1;
        if (neq != 1 || eq_a != reinterpret_cast<const void*>(static_cast<__UINTPTR_TYPE__>(0x7777u)) || eq_b != tinfo) bad = 1;
        const bool base = (ret ^ 1) != 0;
        if (nbase != (base ? 1 : 0)) bad = 1;
        if (base && (base_self != arena + SELF || base_src != arena + SRC || base_flag != flag || base_amt != amt)) bad = 1;
    }
    return bad;
}
