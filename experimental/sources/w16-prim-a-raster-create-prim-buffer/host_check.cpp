// Host check only; not a build input. The "new" block is a real host allocation, so the 32-bit
// element word cannot hold its address; the check uses the low-4GB arena of NORRATH_HOST_ARENA.
#define NORRATH_HOST_ARENA 1
#include "create_prim_buffer.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x6000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned new_result, last_ctor, last_added;
static int add_result, new_calls, ctor_calls, add_calls;
static unsigned new_size;
static unsigned char* add_pool;
extern "C" unsigned __builtin_new(unsigned size) { ++new_calls; new_size = size; return new_result; }
extern "C" void __12VIPrimBuffer(unsigned b) { ++ctor_calls; last_ctor = b; }
extern "C" int Add__t6VIPool1ZP12VIPrimBufferRCP12VIPrimBuffer(unsigned char* pool, unsigned* element) {
    ++add_calls; add_pool = pool; last_added = *element; return add_result;
}

int main() {
    for (unsigned nr : {0u, 0x300u}) {
        for (int ar : {-1, 0, 6}) {
            for (int ref : {0, 1, 5}) {
                memset(arena, 0, sizeof arena);
                unsigned char* o = arena + 0x1000;
                new_result = nr; add_result = ar;
                new_calls = ctor_calls = add_calls = 0;
                int count = 41;
                memcpy(arena + 0x300 + 4, &count, 4);
                int r = CreatePrimBuffer__8VIRasteri(o, ref);
                if (new_calls != 1 || new_size != 112) return 1;
                if (nr == 0) {
                    if (r != -1 || ctor_calls != 0 || add_calls != 0) return 2;
                    continue;
                }
                if (ctor_calls != 1 || last_ctor != nr || add_calls != 1) return 3;
                if (add_pool != o + 19320 || last_added != nr) return 4;
                int expect = ar < 0 ? -1 : ar;
                if (r != expect) return 5;
                int got;
                memcpy(&got, arena + 0x300 + 4, 4);
                if (got != ((ar >= 0 && ref) ? 42 : 41)) return 6;
            }
        }
    }
    return 0;
}
