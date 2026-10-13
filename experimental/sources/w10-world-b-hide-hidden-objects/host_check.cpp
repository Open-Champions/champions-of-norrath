// Host contract for hideHiddenObjects__FP10GameObject. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
#include <cstdio>
#include <cstring>
static int calls; static unsigned char* seen;
extern "C" void objectRemoveFromGrid__FP10GameObject(unsigned char* o) { ++calls; seen = o; }
#include "hide_hidden_objects.cpp"
int main() {
    alignas(4) unsigned char obj[600];
    for (unsigned flags = 0; flags != 0x100; ++flags) {
        for (unsigned b = 0; b != 0x10; ++b) {
            std::memset(obj, 0, sizeof obj);
            std::memcpy(obj + 12, &flags, 4);
            obj[570] = static_cast<unsigned char>(b | 0xf0 * (flags & 1));
            calls = 0;
            hideHiddenObjects__FP10GameObject(obj);
            const bool want = (flags & 0x20) && (obj[570] & 0x08);
            if (calls != (want ? 1 : 0)) return 1;
            if (want && seen != obj) return 2;
        }
    }
    std::puts("hideHiddenObjects check passed");
    return 0;
}
