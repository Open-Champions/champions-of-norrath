// Host check (not a plan input). Build:
//   c++ -O1 -c -D__dynamic_cast_2=w16_dynamic_cast_2 -o /tmp/w16_dc2_src.o dynamic_cast_2.cpp
//   c++ -O1 -o /tmp/w16_dc2 host_check.cpp /tmp/w16_dc2_src.o && /tmp/w16_dc2
#include <cstdio>
#include <vector>

extern "C" void* w16_dynamic_cast_2(const void* (*)(), const void* (*)(), int, void*, const void* (*)(), void*);

static std::vector<int> order;
static const void* a1 = reinterpret_cast<const void*>(0x11);
static const void* a2 = reinterpret_cast<const void*>(0x22);
static const void* a3 = reinterpret_cast<const void*>(0x33);
static const void* fn_first() { order.push_back(1); return a1; }
static const void* fn_second() { order.push_back(2); return a2; }
static const void* fn_third() { order.push_back(3); return a3; }

struct Seen { const void *p1, *p2, *p3; int flag; void *o1, *o2; } seen;
extern "C" void* dyncast__C16__user_type_infoiRC9type_infoPvT2T3(const void* first, int flag, const void* second,
                                                                  void* object, const void* third, void* other) {
    seen = {first, second, third, flag, object, other};
    return reinterpret_cast<void*>(0x9999);
}

int main() {
    void* o1 = reinterpret_cast<void*>(0x1000);
    void* o2 = reinterpret_cast<void*>(0x2000);
    void* result = w16_dynamic_cast_2(fn_first, fn_second, -3, o1, fn_third, o2);
    if (result != reinterpret_cast<void*>(0x9999)) return 1;
    if (order != std::vector<int>{1, 2, 3}) return 2;
    if (seen.p1 != a1 || seen.p2 != a2 || seen.p3 != a3 || seen.flag != -3 || seen.o1 != o1 || seen.o2 != o2) return 3;
    // Null object pointers pass through unchanged.
    w16_dynamic_cast_2(fn_first, fn_second, 0, nullptr, fn_third, nullptr);
    if (seen.o1 != nullptr || seen.o2 != nullptr || seen.flag != 0) return 4;
    std::printf("4 __dynamic_cast_2 checks passed\n");
    return 0;
}
