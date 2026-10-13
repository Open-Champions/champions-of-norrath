// Host contract for msg_save__8SkelightPc; not an EE input. The base save is a stub returning a count.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static int nbase, base_count; static void *base_self, *base_out;
extern "C" int msg_save__8CreaturePc(void* s, char* o) { ++nbase; base_self = s; base_out = o; return base_count; }
#include "msg_save.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char obj[0x600], out[0x100], ref[0x100];
    for (int n : {0, 4, 13, 40}) for (unsigned long long flags : {0ull, 0x400000ull, 0x800000ull, 0xc00000ull, 0x1000000ull, 0x100000000ull, 0xffffffffff3fffffull}) for (unsigned char b85 : {0x00, 0x7f, 0x80, 0xff}) {
        for (unsigned i = 0; i < sizeof obj; ++i) obj[i] = static_cast<unsigned char>(i * 5 + 1);
        std::memcpy(obj + 160, &flags, 8); obj[85] = b85;
        std::memset(out, 0xee, sizeof out); std::memcpy(ref, out, sizeof out);
        base_count = n; nbase = 0;
        const int r = msg_save__8SkelightPc(obj, reinterpret_cast<char*>(out));
        if (r != n + 17 || nbase != 1 || base_self != obj || base_out != out) bad = 1;
        const int wide = static_cast<signed char>(b85);
        std::memcpy(ref + n, &wide, 4); std::memcpy(ref + n + 4, obj + 1104, 4);
        std::memcpy(ref + n + 8, obj + 960, 4); std::memcpy(ref + n + 12, obj + 208, 4);
        ref[n + 16] = (flags & 0xc00000ull) != 0;
        if (std::memcmp(out, ref, sizeof out)) bad = 1;
    }
    return bad;
}
