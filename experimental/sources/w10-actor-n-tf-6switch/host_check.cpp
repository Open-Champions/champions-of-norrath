// Host contract for __tf6Switch; not an EE input. Callees record their arguments.
#include "typeinfo.cpp"

#include <cstring>

static int base_calls;
static int rtti_calls;
static const void* obj[4];
static const char* nam[4];
static const void* bas[4];
unsigned char __ti9WorldPart[12];
unsigned char __ti10GameObject[12];
extern "C" void* __tf10GameObject() { ++base_calls; return nullptr; }
extern "C" void* __rtti_si(void* typeinfo, const char* name, const void* base) {
    obj[rtti_calls] = typeinfo; nam[rtti_calls] = name; bas[rtti_calls] = base;
    ++rtti_calls;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
    return typeinfo;
}

int main() {
    // Both objects unregistered: WorldPart first, then Switch.
    if (__tf6Switch() != __ti6Switch) return 1;
    if (base_calls != 1 || rtti_calls != 2) return 2;
    if (obj[0] != __ti9WorldPart || bas[0] != __ti10GameObject || std::strcmp(nam[0], "9WorldPart") != 0) return 3;
    if (obj[1] != __ti6Switch || bas[1] != __ti9WorldPart || std::strcmp(nam[1], "6Switch") != 0) return 4;
    // Switch already registered: nothing more.
    if (__tf6Switch() != __ti6Switch || rtti_calls != 2 || base_calls != 1) return 5;
    // WorldPart already registered, Switch not: only the Switch call.
    std::memset(__ti6Switch, 0, 12);
    if (__tf6Switch() != __ti6Switch || rtti_calls != 3 || base_calls != 1) return 6;
    if (obj[2] != __ti6Switch || bas[2] != __ti9WorldPart) return 7;
    return 0;
}
