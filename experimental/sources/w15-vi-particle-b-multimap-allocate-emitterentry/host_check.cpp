// Host check only; not a build input. The array word holds an arena offset.
#define NORRATH_HOST_ARENA 1
alignas(16) static unsigned char arena[0x2000];
unsigned char* const norrath_host_arena_base = arena;
#include "allocate.cpp"
#include <cstdio>
#include <cstring>
static int new_calls, del_calls, destroy_calls, link_calls, mat_calls, link_arg;
static unsigned long new_size;
static void* new_result; static void* deleted;
static int destroy_result, link_result;
extern "C" void* __builtin_vec_new(unsigned long size) { ++new_calls; new_size = size; return new_result; }
extern "C" void __builtin_vec_delete(void* p) { ++del_calls; deleted = p; }
extern "C" int DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry(void*) { ++destroy_calls; return destroy_result; }
extern "C" int LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi(void*, int n) { ++link_calls; link_arg = n; return link_result; }
extern "C" void __10VIMatrix44RC10VIMatrix44(void* to, const void* from) { ++mat_calls; std::memcpy(to, from, 64); }
static void reset() { new_calls = del_calls = destroy_calls = link_calls = mat_calls = link_arg = 0; deleted = 0; }
int main() {
    unsigned* m = reinterpret_cast<unsigned*>(arena + 0x100);
    unsigned char* old_nodes = arena + 0x400;
    unsigned char* new_nodes = arena + 0x1000;
    bool ok = true;
    // capacity zero: nothing happens
    std::memset(arena, 0x5a, sizeof arena);
    m[0] = 3; m[1] = 0; m[2] = 0; reset(); new_result = new_nodes; destroy_result = link_result = 0;
    if (Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(m) != 0 || new_calls || m[0] != 3 || m[2] != 0) ok = false;
    // first allocation
    std::memset(arena, 0x5a, sizeof arena);
    m[0] = 0; m[1] = 4; m[2] = 0; reset();
    if (Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(m) != 0 || new_size != 448 || m[0] != 1 || m[2] != 0x1000 || link_arg != 1 || destroy_calls || del_calls) ok = false;
    unsigned* n0 = reinterpret_cast<unsigned*>(new_nodes);
    if (n0[0] || n0[1] || n0[2] || n0[3] || n0[4] != 0x5a5a5a5au) ok = false;
    // allocation failure
    m[0] = 0; m[1] = 4; m[2] = 0; reset(); new_result = 0;
    if (Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(m) != -1 || m[2] != 0) ok = false;
    new_result = new_nodes;
    // growth with two live nodes
    std::memset(arena, 0x5a, sizeof arena);
    for (int i = 0; i < 112 * 2; ++i) old_nodes[i] = static_cast<unsigned char>(i * 14 + 1);
    m[0] = 2; m[1] = 4; m[2] = 0x400; reset();
    if (Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(m) != 0 || m[0] != 2 || m[2] != 0x1000 || deleted != old_nodes || mat_calls != 2 || link_arg != 2 || destroy_calls != 1) ok = false;
    for (int i = 1; i < 2; ++i) {
        for (int off = 0; off < 112; ++off) {
            bool copied = off < 20 || off == 32 || off == 33 || off == 34 || off == 35 || off == 36 || off == 37 || off == 38 || off == 39 || (off >= 48 && off < 112);
            if (copied != (new_nodes[112 * i + off] == old_nodes[112 * i + off])) ok = false;
        }
    }
    // growth with a failing destroy returns -1 before the swap
    std::memset(arena, 0x5a, sizeof arena);
    m[0] = 2; m[1] = 4; m[2] = 0x400; reset(); destroy_result = -1;
    if (Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(m) != -1 || m[2] != 0x400 || del_calls || link_calls) ok = false;
    destroy_result = 0;
    // a failing link propagates
    m[0] = 2; m[1] = 4; m[2] = 0x400; reset(); link_result = -2;
    if (Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(m) != -1) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
