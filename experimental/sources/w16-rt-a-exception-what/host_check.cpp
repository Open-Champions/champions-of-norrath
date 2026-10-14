// Host check (not a plan input). Build:
//   c++ -O1 -c -Dwhat__C9exception=w16_what -o /tmp/w16_what_src.o exception_what.cpp
//   c++ -O1 -o /tmp/w16_what host_check.cpp /tmp/w16_what_src.o && /tmp/w16_what
// (the asm label makes the throw stand-in below the symbol __throw_bad_typeid.)
#include <cstdio>

extern "C" const char* w16_what(const void* self);
static const char kName[] = "name";
static const char kOther[] = "other";
static const char* const kNameRecord = kName;
static const char* const kOtherRecord = kOther;
static const void* seen_self;
static int throws;
static int slot_calls;
static const char* const* slot_one(const void* self) { ++slot_calls; seen_self = self; return &kNameRecord; }
static const char* const* slot_zero(const void*) { return &kOtherRecord; }
extern "C" const char* const* throw_stub() __asm__("__throw_bad_typeid");
extern "C" const char* const* throw_stub() { ++throws; return &kOtherRecord; }

int main() {
    using Slot = const char* const* (*)(const void*);
    static const Slot table[] = {slot_zero, slot_one, slot_zero};
    struct Object { const Slot* table; int pad; } object = {table, 0};
    if (w16_what(&object) != kName || slot_calls != 1 || seen_self != &object || throws != 0) return 1;
    if (w16_what(nullptr) != kOther || throws != 1 || slot_calls != 1) return 2;
    std::printf("2 what__C9exception checks passed\n");
    return 0;
}
