// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the +8 block word translates
// to a host object. Covers a zero and a nonzero block, a pool call that clears the word, and every
// flags bit combination.
#define NORRATH_HOST_CONTRACT 1
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char self_object[64], block[16];
static std::string trace;
static bool clear_in_destroy;
static const unsigned TAG = 0x7000;

extern "C" {
char* norrath_host_translate(unsigned address) { return address == TAG ? reinterpret_cast<char*>(block) : nullptr; }
void DestroyPool__t5VISet1ZP5VIWnd(void* self) {
    trace += self == self_object ? "pool;" : "pool?;";
    if (clear_in_destroy) std::memset(self_object + 8, 0, 4);
}
void __builtin_vec_delete(void* p) { trace += p == block ? "vecdel;" : "vecdel?;"; }
void __builtin_delete(void* p) { trace += p == self_object ? "del;" : "del?;"; }
}

int main() {
    bool ok = true;
    for (int has_block = 0; has_block < 2; ++has_block) {
        for (int clear : {0, 1}) {
            for (int flags = 0; flags < 4; ++flags) {
                std::memset(self_object, 0x11, sizeof self_object);
                const unsigned word = has_block ? TAG : 0u;
                std::memcpy(self_object + 8, &word, 4);
                clear_in_destroy = clear != 0;
                trace.clear();
                _$_t5VISet1ZP5VIWnd(self_object, flags);
                std::string want;
                if (has_block) {
                    want += "pool;";
                    if (!clear) want += "vecdel;";
                }
                if (flags & 1) want += "del;";
                if (trace != want) {
                    std::printf("block %d clear %d flags %d: got %s want %s\n", has_block, clear, flags, trace.c_str(), want.c_str());
                    ok = false;
                }
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
