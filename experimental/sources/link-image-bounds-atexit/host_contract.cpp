#include <cstdint>
#include <cstring>
#include <vector>

#define NORRATH_HOST_CONTRACT 1
#include "atexit.cpp"

namespace {
std::vector<const void*> packed;
int malloc_calls;
unsigned malloc_size;
bool fail_malloc;
}

extern "C" {
alignas(8) unsigned char norrath_atexit_reent[512];
alignas(8) unsigned char norrath_atexit_created[136];
unsigned char* _impure_ptr = norrath_atexit_reent;
}

extern "C" unsigned norrath_host_pack(__UINTPTR_TYPE__ bits) {
    packed.push_back(reinterpret_cast<const void*>(bits));
    return static_cast<unsigned>(packed.size());
}

extern "C" __UINTPTR_TYPE__ norrath_host_unpack(unsigned value) {
    return reinterpret_cast<__UINTPTR_TYPE__>(packed[value - 1]);
}

extern "C" void* __wrap_malloc(unsigned size) {
    malloc_calls += 1;
    malloc_size = size;
    if (fail_malloc) return nullptr;
    return norrath_atexit_created;
}

static int fail(int code) { return code; }
static void marker_a() {}
static void marker_b() {}

int main() {
    std::memset(norrath_atexit_reent, 0, sizeof norrath_atexit_reent);
    std::memset(norrath_atexit_created, 0, sizeof norrath_atexit_created);
    unsigned char* slot = norrath_atexit_reent + 328;
    unsigned char* first = norrath_atexit_reent + 332;
    // The call is indirect so the compiler cannot delete the stores it
    // cannot see through _impure_ptr.
    int (*volatile invoke)(void (*)()) = atexit;
    if (invoke(marker_a) != 0) return fail(1);
    if (malloc_calls != 0 || load32(slot) == 0) return fail(2);
    if (reinterpret_cast<unsigned char*>(norrath_host_unpack(load32(slot))) != first) return fail(3);
    if (static_cast<int>(load32(first + 4)) != 1) return fail(4);
    if (norrath_host_unpack(load32(first + 8)) != reinterpret_cast<__UINTPTR_TYPE__>(marker_a)) return fail(5);

    for (int i = 1; i < 32; ++i) {
        if (invoke(marker_a) != 0) return fail(6);
    }
    if (malloc_calls != 0 || static_cast<int>(load32(first + 4)) != 32) return fail(7);

    if (invoke(marker_b) != 0) return fail(8);
    if (malloc_calls != 1 || malloc_size != 136) return fail(9);
    auto* second = reinterpret_cast<unsigned char*>(norrath_host_unpack(load32(slot)));
    if (second == first || static_cast<int>(load32(second + 4)) != 1) return fail(10);
    if (reinterpret_cast<unsigned char*>(norrath_host_unpack(load32(second))) != first) return fail(11);
    if (norrath_host_unpack(load32(second + 8)) != reinterpret_cast<__UINTPTR_TYPE__>(marker_b)) return fail(12);
    if (static_cast<int>(load32(first + 4)) != 32) return fail(13);

    if (invoke(marker_a) != 0) return fail(14);
    if (malloc_calls != 1 || static_cast<int>(load32(second + 4)) != 2) return fail(15);

    fail_malloc = true;
    store32(second + 4, 32);
    unsigned kept = load32(slot);
    if (invoke(marker_b) != -1) return fail(16);
    if (load32(slot) != kept || static_cast<int>(load32(second + 4)) != 32) return fail(17);
    return 0;
}
