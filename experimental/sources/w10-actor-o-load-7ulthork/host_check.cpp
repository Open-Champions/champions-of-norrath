// Host contract for msg_load__7UlthorkPc; not an EE input. Includes the source. 32-bit words are
// offsets from a host base.
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "msg_load.cpp"

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static unsigned char* const object = arena + 0x400;
static unsigned char* const buffer = arena + 0x1800;
static int base_calls, base_return, chunk_calls;
static void* chunk_model; static int chunk_which;
extern "C" int msg_load__8CreaturePc(void* self, char*) { ++base_calls; if (self != object) base_calls += 100; return base_return; }
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int which) {
    ++chunk_calls; chunk_model = model; chunk_which = which; return 0x1122334455667788ull;
}
static void put32(unsigned char* base, unsigned o, unsigned v) { std::memcpy(base + o, &v, 4); }

int main() {
    for (int n : {0, 4, 12}) {
        for (unsigned b : {0x00u, 0x01u, 0x02u, 0x82u, 0x102u, 0xffffff02u, 0x03u}) {
            for (int half : {10, 9, 6, -1}) {
                std::memset(arena, 0x33, sizeof arena); w10_actor_o_host_base = arena;
                put32(object, 36, 0x1000);       // group block
                put32(arena + 0x1000, 12, 0x1234);
                put32(buffer, n, 0xcafe0001u); put32(buffer, n + 4, b);
                short h = static_cast<short>(half); std::memcpy(object + 572, &h, 2);
                base_calls = 0; chunk_calls = 0; base_return = n;
                if (msg_load__7UlthorkPc(object, reinterpret_cast<char*>(buffer)) != n + 8) return 1;
                if (base_calls != 1 || chunk_calls != 1) return 2;
                const bool two = static_cast<signed char>(b) == 2;
                if (chunk_model != arena + 0x1234 || chunk_which != (two ? 1 : 0)) return 3;
                unsigned w960; std::memcpy(&w960, object + 960, 4);
                if (w960 != (half == 10 ? 0u : 0xcafe0001u)) return 4;
                if (object[156] != (two ? 6 : 0) || object[1104] != static_cast<unsigned char>(b)) return 5;
                unsigned w244; std::memcpy(&w244, object + 244, 4);
                if (w244 != (two ? 1u : 0u)) return 6;
                unsigned long long chunks; std::memcpy(&chunks, object + 552, 8);
                if (chunks != 0x1122334455667788ull) return 7;
            }
        }
    }
    return 0;
}
