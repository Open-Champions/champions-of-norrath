#include <cstring>

extern "C" void* __tfP8ItemProp();
extern "C" unsigned char norrath_ti_P8ItemProp[12];
extern "C" unsigned char norrath_ti_8ItemProp[12] = {};

static int item_calls, ptr_calls;
static void* seen_object;
static const char* seen_name;
static void* seen_target;

extern "C" void* __tf8ItemProp() { ++item_calls; return norrath_ti_8ItemProp; }
extern "C" void norrath_rtti_ptr(void* typeinfo, const char* name, void* target) {
    ++ptr_calls;
    seen_object = typeinfo;
    seen_name = name;
    seen_target = target;
    static_cast<unsigned char*>(typeinfo)[0] = 3;
}

int main() {
    void* first = __tfP8ItemProp();
    if (first != norrath_ti_P8ItemProp) return 1;
    if (item_calls != 1 || ptr_calls != 1) return 2;
    if (seen_object != norrath_ti_P8ItemProp || seen_target != norrath_ti_8ItemProp) return 3;
    if (!seen_name || std::strcmp(seen_name, "P8ItemProp") != 0) return 4;
    if (__tfP8ItemProp() != first || item_calls != 1 || ptr_calls != 1) return 5;
    std::memset(norrath_ti_P8ItemProp, 0, 12);
    norrath_ti_P8ItemProp[6] = 1;
    __tfP8ItemProp();
    if (item_calls != 2 || ptr_calls != 2) return 6;
    return 0;
}
