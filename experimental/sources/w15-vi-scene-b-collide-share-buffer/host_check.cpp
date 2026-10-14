// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "share.cpp"
#include <cstdint>
static unsigned char arena[0x2000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
int main() {
    unsigned char* owner = arena;                  // the owning record
    unsigned char* records = arena + 0x800;        // 12-byte records
    unsigned char* objects = arena + 0x1000;       // shared objects
    *reinterpret_cast<unsigned*>(owner + 536) = word_of(records);
    for (int i = 0; i < 4; ++i) {
        *reinterpret_cast<unsigned*>(records + 12 * i) = word_of(objects + 0x40 * i);
        *reinterpret_cast<int*>(objects + 0x40 * i + 4) = 10 * i;
        *reinterpret_cast<unsigned*>(records + 12 * i + 4) = 0x1111;
    }
    if (ShareCollBuffer__9VICollidei(owner, 2) != 0) return 1;
    if (ShareCollBuffer__9VICollidei(owner, 2) != 0) return 2;
    if (ShareCollBuffer__9VICollidei(owner, 0) != 0) return 3;
    const int want[4] = {1, 10, 22, 30};
    for (int i = 0; i < 4; ++i) if (*reinterpret_cast<int*>(objects + 0x40 * i + 4) != want[i]) return 4 + i;
    if (*reinterpret_cast<unsigned*>(records + 4) != 0x1111) return 8;
    return 0;
}
