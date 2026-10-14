// Host check only; not a build input.
#include "clear.cpp"
#include <cstring>
static int log_[8], n_, destroyRet, zeroIt;
extern "C" int DestroyPool__t7VIArray1Z10VIZoneLeaf(void* a) { log_[n_++] = 1; if (zeroIt) static_cast<unsigned*>(a)[1] = 0; return destroyRet; }
extern "C" void __builtin_vec_delete(void* p) { log_[n_++] = (reinterpret_cast<unsigned long>(p) == 0x1000) ? 2 : 99; }
int main() {
    unsigned o[6];
    struct { unsigned arr; int ret; int zero; int calls; int result; bool stored; } cases[] = {
        {0, 0, 0, 0, 0, true}, {0x1000, 0, 0, 2, 0, true}, {0x1000, 5, 0, 2, 0, true}, {0x1000, 0, 1, 1, 0, true},
        {0x1000, -1, 0, 1, -1, false}, {0x1000, -9, 1, 1, -1, false}};
    for (auto& c : cases) {
        std::memset(o, 0x5a, sizeof o);
        o[0] = 0x77; o[1] = c.arr; n_ = 0; destroyRet = c.ret; zeroIt = c.zero;
        if (Clear__t7VIArray1Z10VIZoneLeaf(o) != c.result) return 1;
        if (n_ != c.calls) return 2;
        if (c.calls == 2 && (log_[0] != 1 || log_[1] != 2)) return 3;
        if (c.stored) { if (o[1] != 0 || o[0] != 0) return 4; }
        else { if (o[1] != (c.zero ? 0u : 0x1000u) || o[0] != 0x77) return 5; }
        for (int i = 2; i < 6; ++i) if (o[i] != 0x5a5a5a5au) return 6;
    }
    return 0;
}
