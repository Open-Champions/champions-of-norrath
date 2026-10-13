// Host contract for __tf10StaticFire; not an EE input. Callees record their arguments.
#include "typeinfo.cpp"

#include <cstring>

static int base_calls;
static int rtti_calls;
static void* seen_object;
static const char* seen_name;
static const void* seen_base;
unsigned char __ti10GameObject[12];
extern "C" void* __tf10GameObject() { ++base_calls; return nullptr; }
extern "C" void* __rtti_si(void* typeinfo, const char* name, const void* base) {
    ++rtti_calls;
    seen_object = typeinfo;
    seen_name = name;
    seen_base = base;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
    return typeinfo;
}

int main() {
    std::memset(__ti10StaticFire, 0, 12);
    if (__tf10StaticFire() != __ti10StaticFire) return 1;
    if (base_calls != 1 || rtti_calls != 1) return 2;
    if (seen_object != __ti10StaticFire || seen_base != __ti10GameObject) return 3;
    if (std::strcmp(seen_name, "10StaticFire") != 0) return 4;
    if (__tf10StaticFire() != __ti10StaticFire) return 5;
    if (base_calls != 1 || rtti_calls != 1) return 6;
    std::memset(__ti10StaticFire, 0, 12);
    __ti10StaticFire[4] = 9;
    if (__tf10StaticFire() != __ti10StaticFire || rtti_calls != 2 || base_calls != 2) return 7;
    return 0;
}
