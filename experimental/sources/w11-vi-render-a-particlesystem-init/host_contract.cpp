#include "init.cpp"
#include <string.h>

static unsigned char heap[1200];
static unsigned int requested;
static int new_fails;
static unsigned char* nodes_seen[8]; static int node_n;
static unsigned char* tails[8]; static int tail_n;
static unsigned char* list_seen; static int init_n;
static int order[32]; static int order_n;
extern "C" void* __builtin_vec_new(unsigned int size) { requested = size; order[order_n++] = 1; return new_fails ? 0 : heap; }
extern "C" void __7VILNode(void* n) { nodes_seen[node_n++] = static_cast<unsigned char*>(n); order[order_n++] = 2; }
extern "C" void Init__7VILList(void* l) { list_seen = static_cast<unsigned char*>(l); ++init_n; order[order_n++] = 3; }
extern "C" void AddTail__7VILListP7VILNode(void*, void* n) { tails[tail_n++] = static_cast<unsigned char*>(n); order[order_n++] = 4; }

static unsigned int word(unsigned char* b, int o) { unsigned int v; memcpy(&v, b + o, 4); return v; }
static unsigned int addr(void* p) { return static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

int main() {
    static unsigned char object[128];
    static int raster;
    for (int count = 0; count <= 3; count += 3) {
        memset(heap, 0xcc, sizeof heap);
        memset(object, 0x77, sizeof object);
        node_n = tail_n = init_n = order_n = 0; new_fails = 0;
        int r = Init__16VIParticleSystemlP8VIRasteri(object, 0x1122334455667788LL, &raster, count);
        if (r != 0) return 1;
        if (requested != static_cast<unsigned>(count * 104)) return 2;
        long long v; memcpy(&v, object + 8, 8);
        if (v != 0x1122334455667788LL) return 3;
        if (word(object, 0) != addr(&raster) || word(object, 76) != static_cast<unsigned>(count)) return 4;
        if (word(object, 80) != addr(heap)) return 5;
        if (node_n != count || tail_n != count || init_n != 1 || list_seen != object + 64) return 6;
        for (int i = 0; i < count; ++i) {
            if (nodes_seen[i] != heap + 104 * i || tails[i] != heap + 104 * i) return 7;
            static const int zero_ranges[4][2] = {{32, 12}, {44, 12}, {56, 16}, {72, 16}};
            for (int k = 0; k < 4; ++k)
                for (int b = 0; b < zero_ranges[k][1]; ++b)
                    if (heap[104 * i + zero_ranges[k][0] + b] != 0) return 8;
            // untouched bytes of the record stay as they were
            if (heap[104 * i + 0] != 0xcc || heap[104 * i + 28] != 0xcc || heap[104 * i + 88] != 0xcc) return 9;
        }
        for (int o = 20; o <= 48; o += 4) if (word(object, o) != 0x3d088888u) return 10;
        if (word(object, 16) != 0 || word(object, 12) != 0x11223344u || word(object, 52) != 0x77777777u) return 11;
        if (word(object, 4) != 0x77777777u || word(object, 60) != 0x77777777u) return 12;
        if (count == 3) {
            // vector new is called first; nodes follow, then the list init, then the tails
            int expect[] = {1, 2, 2, 2, 3, 4, 4, 4};
            if (order_n != 8) return 13;
            for (int i = 0; i < 8; ++i) if (order[i] != expect[i]) return 14;
        }
    }
    // failed allocation: the pointer word is zero, no nodes are built, the tail loop still runs
    memset(object, 0x77, sizeof object);
    node_n = tail_n = init_n = order_n = 0; new_fails = 1;
    Init__16VIParticleSystemlP8VIRasteri(object, 5, &raster, 2);
    if (word(object, 80) != 0 || node_n != 0 || init_n != 1 || tail_n != 2) return 20;
    if (tails[0] != 0 || tails[1] != reinterpret_cast<unsigned char*>(104)) return 21;
    return 0;
}
