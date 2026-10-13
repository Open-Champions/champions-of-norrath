// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "query.cpp"
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

static unsigned char *swC, *swM, *qC, *qB, *qL; static int qO, order, swOrder, qOrder, ret;
extern "C" void SetWorld__9VICollideRC10VIMatrix44(unsigned char* c, unsigned char* m) { swC = c; swM = m; swOrder = ++order; }
extern "C" int QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi(unsigned char* c, unsigned char* b, unsigned char* l, int o) { qC = c; qB = b; qL = l; qO = o; qOrder = ++order; return ret; }
int main() {
    unsigned char* self = arena + 0x100; unsigned char* scene = arena + 0x1000; unsigned char* coll = arena + 0x8000;
    unsigned char* box = arena + 0x9000; unsigned char* list = arena + 0x9100; unsigned char* mat = arena + 0x9200;
    for (int k = 0; k < 4; ++k) for (int r = 0; r < 2; ++r) {
        for (unsigned i = 0; i < 0x10000; ++i) arena[i] = static_cast<unsigned char>(i * 7 + 3);
        *reinterpret_cast<unsigned*>(scene + 19364) = word_of(coll + 16 * k);
        *reinterpret_cast<int*>(self + 68) = 0x1234 * k - 77;
        order = 0; ret = r ? -9 : 41;
        int got = QueryIntersection__14VISimpleSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene(self, box, list, mat, scene);
        if (got != ret) return 1;
        if (swC != coll + 16 * k || swM != mat || qC != coll + 16 * k || qB != box || qL != list) return 2;
        if (qO != 0x1234 * k - 77) return 3;
        if (swOrder != 1 || qOrder != 2) return 4;
    }
    return 0;
}
