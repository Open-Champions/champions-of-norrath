// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
// The pointer field is pointer-sized on the host, so only identity and null are checked.
#include "trailfx-clear.cpp"
#include <cstring>

static void* freed;
static int frees;
static void* inits[4];
static int init_count;
extern "C" void __builtin_vec_delete(void* pointer) { freed = pointer; ++frees; }
extern "C" void Init__7VILList(void* list) { inits[init_count++] = list; }

int main() {
    alignas(16) unsigned char fx[96];
    std::memset(fx, 0x33, sizeof fx);
    void* array = fx + 8;
    std::memcpy(fx + 52, &array, sizeof array);
    if (Clear__9VITrailFx(fx) != 0) return 1;
    if (frees != 1 || freed != array) return 2;
    void* now;
    std::memcpy(&now, fx + 52, sizeof now);
    if (now != 0) return 3;
    if (init_count != 2 || inits[0] != fx + 28 || inits[1] != fx + 40) return 4;
    frees = 0; init_count = 0;
    if (Clear__9VITrailFx(fx) != 0 || frees != 0 || init_count != 2) return 5;
    return 0;
}
