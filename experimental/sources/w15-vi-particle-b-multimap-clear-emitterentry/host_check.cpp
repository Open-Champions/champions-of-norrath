// Host check only; not a build input. The array word holds an arena offset.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x1000];
unsigned char* const norrath_host_arena_base = arena;
#include "clear.cpp"
#include <cstdio>
static int destroy_calls, delete_calls, destroy_result;
static void* deleted;
extern "C" int DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(void*) { ++destroy_calls; return destroy_result; }
extern "C" void __builtin_vec_delete(void* p) { ++delete_calls; deleted = p; }
int main() {
    unsigned* m = reinterpret_cast<unsigned*>(arena + 0x100);
    bool ok = true;
    for (int array = 0; array < 2; ++array) {
        for (int res = 0; res < 3; ++res) {
            for (int i = 0; i < 6; ++i) m[i] = 0x1111u * (i + 1);
            m[2] = array ? 0x200 : 0;
            destroy_calls = delete_calls = 0; deleted = 0;
            destroy_result = res == 0 ? -1 : res == 1 ? 0 : 7;
            int r = Clear__t10VIMultiMap2ZfZ14VIEmitterEntry(m);
            if (array && res == 0) {
                if (r != -1 || destroy_calls != 1 || delete_calls != 0 || m[0] != 0x1111u || m[2] != 0x200 || m[4] != 0x5555u) ok = false;
                continue;
            }
            if (r != 0 || destroy_calls != array || delete_calls != array) ok = false;
            if (array && deleted != arena + 0x200) ok = false;
            if (m[0] || m[1] || m[2] || m[3] || m[4] != 0xffffffffu || m[5] != 0x6666u) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
