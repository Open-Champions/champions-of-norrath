// Host contract for _$_7VILList; not an EE input. Callees record their arguments; the object is a local array.
#include "dtor.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x200];
static std::string trace;
static long off(const void* p) { return static_cast<const unsigned char*>(p) - object; }

void free_object(void* pointer) { trace += "delete@" + std::to_string(off(pointer)) + ";"; }

int main() {
    int failures = 0;
    for (unsigned f : {0u, 1u, 2u, 3u, 0x80000001u, 0xfffffffeu}) {
        const int flags = static_cast<int>(f);
        std::memset(object, 0x3c, sizeof object);
        trace.clear();
        destructor_7VILList(object, flags);
        std::string want;
        if (flags & 1) want += "delete@0;";
        if (trace != want) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", flags, trace.c_str(), want.c_str()); ++failures; }
    }
    if (!failures) std::puts("w14-m-sprite-dtors-dtor-7villist ok");
    return failures ? 1 : 0;
}
