// Host contract for getModel__11VampireLord; not an EE input. 32-bit words are offsets from a host base.
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "get_model.cpp"

static unsigned char arena[0x1000] __attribute__((aligned(16)));
static std::string calls;
static int token;
extern "C" void* lumpFindResource__FPcT0(const char* a, const char* b) { calls += std::string("lump:") + a + ":" + b + ";"; return &token; }
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
int main() {
    w10_actor_p_host_base = arena;
    unsigned char* const obj = arena + 0x400;
    put32(0x300 + 12, 0xabcd1234u);
    for (unsigned flag : {0u, 1u, 0x80000000u}) {
        calls.clear();
        std::memset(obj, 0, 1000);
        std::memcpy(obj + 964, &flag, 4);
        unsigned w = 0x300; std::memcpy(obj + 36, &w, 4);
        void* r = getModel__11VampireLord(obj);
        if (flag != 0) {
            if (r != &token || calls != "lump:bat.lmp:bat.vif;") { std::puts(calls.c_str()); return 1; }
        } else if (reinterpret_cast<__UINTPTR_TYPE__>(r) != 0xabcd1234u || !calls.empty()) {
            return 2;
        }
    }
    return 0;
}
