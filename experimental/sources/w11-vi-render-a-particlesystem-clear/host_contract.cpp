#include "clear.cpp"
#include <string.h>

static unsigned char* pool_base;
static unsigned char* list_inits[16]; static int init_n;
static unsigned char* tails[8]; static int tail_n;
static int release_n; static void* release_dict;
static void* freed; static int free_n;
static int order[40]; static int order_n;
extern "C" void Init__7VILList(void* l) { list_inits[init_n++] = static_cast<unsigned char*>(l); order[order_n++] = 1; }
extern "C" void AddTail__7VILListP7VILNode(void* l, void* n) {
    if (reinterpret_cast<unsigned char*>(l) != pool_base + 64) order[order_n++] = 99;
    tails[tail_n++] = static_cast<unsigned char*>(n); order[order_n++] = 2;
}
extern "C" int ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary(void*, void* d) { ++release_n; release_dict = d; order[order_n++] = 3; return 0; }
extern "C" void __builtin_vec_delete(void* p) { freed = p; ++free_n; order[order_n++] = 4; }

static unsigned int word(unsigned char* b, int o) { unsigned int v; memcpy(&v, b + o, 4); return v; }
static void put(unsigned char* b, int o, unsigned int v) { memcpy(b + o, &v, 4); }
static unsigned int addr(void* p) { return static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

int main() {
    static unsigned char system[256];
    static unsigned char nodes[3][256];
    static unsigned char records[400];
    pool_base = system;
    memset(system, 0, sizeof system); memset(nodes, 0, sizeof nodes);
    put(system, 52, addr(nodes[0]));
    put(nodes[0], 0, addr(nodes[1]));
    put(nodes[1], 0, addr(nodes[2]));
    put(nodes[0], 80, 7); put(nodes[0], 84, 8); put(nodes[0], 172, 9);
    put(nodes[1], 80, 0); put(nodes[1], 84, 6); put(nodes[1], 172, 5);
    put(nodes[2], 80, 3); put(nodes[2], 84, 3);   // tail node is never touched
    put(system, 76, 3); put(system, 80, addr(records));
    int dict;
    int r = Clear__16VIParticleSystemR12VIDictionary(system, &dict);
    if (r != 0) return 1;
    if (word(nodes[0], 80) != 0 || word(nodes[0], 84) != 0 || word(nodes[0], 172) != 0) return 2;
    if (word(nodes[1], 84) != 6 || word(nodes[1], 172) != 5) return 3;
    if (word(nodes[2], 80) != 3) return 4;
    if (init_n != 3) return 5;
    if (list_inits[0] != nodes[0] + 160 || list_inits[1] != system + 52 || list_inits[2] != system + 64) return 6;
    if (tail_n != 3 || tails[0] != records || tails[1] != records + 104 || tails[2] != records + 208) return 7;
    if (release_n != 1 || release_dict != &dict) return 8;
    if (free_n != 1 || freed != records) return 9;
    if (word(system, 80) != 0 || word(system, 76) != 0) return 10;
    if (word(system, 52) != addr(nodes[0])) return 11;
    int expect_order[] = {1, 1, 1, 2, 2, 2, 3, 4};
    if (order_n != 8) return 12;
    for (int i = 0; i < 8; ++i) if (order[i] != expect_order[i]) return 20 + i;

    // zero records: nothing added, nothing freed, count cleared
    init_n = tail_n = release_n = free_n = order_n = 0;
    put(nodes[0], 0, 0);
    put(system, 76, 0); put(system, 80, 0);
    if (Clear__16VIParticleSystemR12VIDictionary(system, &dict) != 0) return 40;
    if (init_n != 2 || tail_n != 0 || free_n != 0 || release_n != 1) return 41;
    if (word(system, 76) != 0 || word(system, 80) != 0) return 42;
    // a negative count adds nothing and is cleared
    init_n = tail_n = release_n = free_n = 0;
    put(system, 76, 0xfffffffe); put(system, 80, addr(records));
    Clear__16VIParticleSystemR12VIDictionary(system, &dict);
    if (tail_n != 0 || free_n != 1 || word(system, 76) != 0) return 43;
    return 0;
}
