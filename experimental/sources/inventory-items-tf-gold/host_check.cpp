#include <cstring>

extern "C" void* __tf4Gold();
extern "C" unsigned char norrath_ti_4Gold[12];
extern "C" unsigned char norrath_ti_10GameObject[8] = {};

static int base_calls, rtti_calls;
static void* seen_object;
static const char* seen_name;
static void* seen_base;

extern "C" void* norrath_vt_tf_10GameObject() { ++base_calls; return norrath_ti_10GameObject; }
extern "C" void norrath_rtti_si(void* typeinfo, const char* name, void* base) {
    ++rtti_calls;
    seen_object = typeinfo;
    seen_name = name;
    seen_base = base;
    static_cast<unsigned char*>(typeinfo)[0] = 2;
}

int main() {
    void* first = __tf4Gold();
    if (first != norrath_ti_4Gold) return 1;
    if (base_calls != 1 || rtti_calls != 1) return 2;
    if (seen_object != norrath_ti_4Gold || seen_base != norrath_ti_10GameObject) return 3;
    if (!seen_name || std::strcmp(seen_name, "4Gold") != 0) return 4;
    if (__tf4Gold() != first || base_calls != 1 || rtti_calls != 1) return 5;
    // Only the first word gates; a nonzero later byte alone does not.
    std::memset(norrath_ti_4Gold, 0, 12);
    norrath_ti_4Gold[4] = 9;
    base_calls = rtti_calls = 0;
    __tf4Gold();
    if (base_calls != 1 || rtti_calls != 1) return 6;
    std::memset(norrath_ti_4Gold, 0, 12);
    norrath_ti_4Gold[3] = 1;
    base_calls = rtti_calls = 0;
    __tf4Gold();
    if (base_calls != 0 || rtti_calls != 0) return 7;
    return 0;
}
