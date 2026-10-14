// Host contract for __tf5Torch; not an EE input. Callees record their arguments.
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
    std::memset(__ti5Torch, 0, 12);
    if (__tf5Torch() != __ti5Torch) return 1;
    if (base_calls != 1 || rtti_calls != 1) return 2;
    if (seen_object != __ti5Torch || seen_base != __ti10GameObject) return 3;
    if (std::strcmp(seen_name, "5Torch") != 0) return 4;
    if (__tf5Torch() != __ti5Torch) return 5;
    if (base_calls != 1 || rtti_calls != 1) return 6;
    std::memset(__ti5Torch, 0, 12);
    __ti5Torch[4] = 9;
    if (__tf5Torch() != __ti5Torch || rtti_calls != 2 || base_calls != 2) return 7;
    return 0;
}
