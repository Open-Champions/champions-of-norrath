#include "clear.cpp"
#include <cstring>
static int log_[8], n_, destroyRet, zeroIt;
static unsigned* cur;
extern "C" int DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* a) { log_[n_++] = 1; if (zeroIt) static_cast<unsigned*>(a)[2] = 0; return destroyRet; }
extern "C" void __builtin_vec_delete(void* p) { log_[n_++] = (p == reinterpret_cast<void*>(0x1000)) ? 2 : 99; }
int main() {
    unsigned o[6];
    struct { unsigned arr; int ret; int zero; int calls; int result; bool stored; } cases[] = {
        {0, 0, 0, 0, 0, true}, {0x1000, 0, 0, 2, 0, true}, {0x1000, 5, 0, 2, 0, true}, {0x1000, 0, 1, 1, 0, true},
        {0x1000, -1, 0, 1, -1, false}, {0x1000, -9, 1, 1, -1, false}};
    for (auto& c : cases) {
        std::memset(o, 0x5a, sizeof o);
        o[2] = c.arr; n_ = 0; destroyRet = c.ret; zeroIt = c.zero;
        if (Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(o) != c.result) return 1;
        if (n_ != c.calls) return 2;
        if (c.calls == 2 && (log_[0] != 1 || log_[1] != 2)) return 3;
        if (c.stored) {
            if (o[2] != 0) return 4;
        if (o[0] != 0) return 20 + 0;
        if (o[1] != 0) return 20 + 1;
        if (o[2] != 0) return 20 + 2;
        if (o[3] != 0) return 20 + 3;
        if (o[4] != 0xffffffffu) return 30 + 4;
        } else {
            if (o[2] != (c.zero ? 0u : 0x1000u)) return 5;
        }
        if (o[5] != 0x5a5a5a5au) return 40 + 5;
    }
    return 0;
}
