// Host contract for _$_8Skeleton; not an EE input. Callees log in order; the table pointer is
// inspected when the first callee runs.
#include <cstring>
#include <initializer_list>
#include <string>
#include <cstdint>
static std::string order; static unsigned char* g_obj; static unsigned seen_vt; static int base_flags = -1;
extern "C" unsigned char norrath_ctors_lava_wraith_view_44fee8[240] = {};
extern "C" void listDestroy__FP8ListHead(void* h) { if (order.empty()) std::memcpy(&seen_vt, g_obj + 72, 4); order += "L" + std::to_string(static_cast<unsigned char*>(h) - g_obj) + ";"; }
extern "C" void _$_8Creature(void* o, int f) { order += "C;"; base_flags = f; if (o != g_obj) order += "BAD;"; }
extern "C" void blockFree__FPv(void* o) { order += "F;"; if (o != g_obj) order += "BAD;"; }
#include "destructor.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char obj[0x600];
    g_obj = obj;
    for (int flags : {0, 1, 2, 3, 0x10, 0x11, -1, -2, 0x7fffffff}) {
        std::memset(obj, 0x6b, sizeof obj); order.clear(); base_flags = -1; seen_vt = 0;
        unsigned char ref[0x600]; std::memcpy(ref, obj, sizeof ref);
        const unsigned vt = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_lava_wraith_view_44fee8));
        std::memcpy(ref + 72, &vt, 4);
        _$_8Skeleton(obj, flags);
        if (seen_vt != vt || base_flags != 0) bad = 1;
        if (order != std::string("L964;L960;C;") + ((flags & 1) ? "F;" : "")) bad = 1;
        if (std::memcmp(obj, ref, sizeof obj)) bad = 1;
    }
    return bad;
}
