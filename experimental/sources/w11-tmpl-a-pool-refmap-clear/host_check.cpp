#include "clear.cpp"
#include <cstring>
static int log_[8], n_;
static int clearArray;
static unsigned* current;
extern "C" void DestroyPool__t6VIPool1ZP8VIRefMap(void* pool) { log_[n_++] = 1; if (clearArray) static_cast<unsigned*>(pool)[2] = 0; }
extern "C" void __builtin_vec_delete(void* p) { log_[n_++] = 2; if (p != reinterpret_cast<void*>(0x1000)) log_[n_++] = 99; }
int main() {
    unsigned pool[8];
    for (int mode = 0; mode < 3; ++mode) {
        std::memset(pool, 0x5a, sizeof pool);
        n_ = 0; clearArray = (mode == 2);
        pool[2] = (mode == 0) ? 0 : 0x1000;
        if (Clear__t6VIPool1ZP8VIRefMap(pool) != 0) return 1;
        int want[3] = {0, 2, 1};
        if (n_ != want[mode]) return 2;
        if (mode == 1 && (log_[0] != 1 || log_[1] != 2)) return 3;
        if (mode == 2 && log_[0] != 1) return 4;
        if (pool[0] || pool[1] || pool[2] || pool[3] != 0xffffffffu || pool[4] != 0xffffffffu || pool[5] != 0xffffffffu) return 5;
        if (pool[6] != 0x5a5a5a5au || pool[7] != 0x5a5a5a5au) return 6;
    }
    return 0;
}
