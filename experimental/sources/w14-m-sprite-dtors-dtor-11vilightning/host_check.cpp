// Host contract for _$_11VILightning; not an EE input. Callees record their arguments; the object is a local array.
// The -112 sentinel is passed as a 64-bit value, so the host pointer is not used for it.
#include "dtor.cpp"
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string>

static unsigned char object[0x2000];
static std::string trace;
static long off(const void* p) { return static_cast<const unsigned char*>(p) - object; }
void matrix_destructor(void* self, int flags) { trace += "m@" + std::to_string(off(self)) + "," + std::to_string(flags) + ";"; }
void free_object(void* pointer) { trace += "delete@" + std::to_string(off(pointer)) + ";"; }

int main() {
    int failures = 0;
    for (unsigned f : {0u, 1u, 2u, 3u, 0x80000001u, 0xfffffffeu}) {
        const int flags = static_cast<int>(f);
        trace.clear();
        destructor_11VILightning(object, flags);
        std::string want = "m@6256,2;m@4704,2;m@3152,2;m@1600,2;m@32,2;";
        if (flags & 1) want += "delete@0;";
        if (trace != want) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", flags, trace.c_str(), want.c_str()); ++failures; }
    }
    // Sentinel case: the loop is skipped, so one matrix call (on the sentinel plus 32) and the delete remain.
    trace.clear();
    destructor_11VILightning(reinterpret_cast<void*>(static_cast<std::intptr_t>(-112)), 1);
    const std::string calls = trace.substr(0, trace.find("delete@"));
    if (calls.find("m@") != 0 || calls.find(";", 0) == std::string::npos || calls.find("m@", 1) != std::string::npos ||
        trace.find("delete@") == std::string::npos) {
        std::fprintf(stderr, "fail: sentinel case %s\n", trace.c_str());
        ++failures;
    }
    if (!failures) std::puts("w14-m-sprite-dtors-dtor-11vilightning ok");
    return failures ? 1 : 0;
}
