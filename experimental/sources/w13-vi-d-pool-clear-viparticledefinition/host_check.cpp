// Host check only; not a build input.
#include "clear.cpp"
#include <cstring>
static int log_[8], n_, zeroIt;
extern "C" void DestroyPool__t6VIPool1ZP20VIParticleDefinition(void* a) { log_[n_++] = 1; if (zeroIt) static_cast<unsigned*>(a)[2] = 0; }
extern "C" void __builtin_vec_delete(void* p) { log_[n_++] = (reinterpret_cast<unsigned long>(p) == 0x1000) ? 2 : 99; }
int main() {
    unsigned o[8];
    struct { unsigned storage; int zero; int calls; } cases[] = {{0, 0, 0}, {0x1000, 0, 2}, {0x1000, 1, 1}};
    for (auto& c : cases) {
        std::memset(o, 0x5a, sizeof o);
        o[2] = c.storage; n_ = 0; zeroIt = c.zero;
        if (Clear__t6VIPool1ZP20VIParticleDefinition(o) != 0) return 1;
        if (n_ != c.calls) return 2;
        if (c.calls == 2 && (log_[0] != 1 || log_[1] != 2)) return 3;
        const unsigned want[6] = {0, 0, 0, 0xffffffffu, 0xffffffffu, 0xffffffffu};
        for (int i = 0; i < 6; ++i) if (o[i] != want[i]) return 4;
        if (o[6] != 0x5a5a5a5au || o[7] != 0x5a5a5a5au) return 5;
    }
    return 0;
}
