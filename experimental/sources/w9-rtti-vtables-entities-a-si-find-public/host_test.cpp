// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include "si_find_public.cpp"

static int calls, last_offset;
static const void* last_self;
static void* last_object;
static void* last_sub;
extern "C" int __eq__C9type_infoRC9type_info(const void* l, const void* r) {
    return l == r || std::strcmp(*static_cast<const char* const*>(l), *static_cast<const char* const*>(r)) == 0;
}
static int forwarded(const void* self, int offset, const void*, void* object, void* sub) {
    ++calls; last_self = self; last_offset = offset; last_object = object; last_sub = sub;
    return 1234;
}

int main() {
    NorrathTypeInfoSlot table[6] = {};
    table[4].adjustment = 16;
    table[4].function = reinterpret_cast<void*>(forwarded);
    table[3].function = reinterpret_cast<void*>(+[](const void*, int, const void*, void*, void*) { return -1; });
    char base_storage[64] = {};
    NorrathSiTypeInfo base = {"base", table, nullptr};
    NorrathSiTypeInfo self = {"abc", table, &base};
    NorrathSiTypeInfo same_name = {"abc", table, nullptr};
    NorrathSiTypeInfo other = {"xyz", table, nullptr};
    (void)base_storage;
    char buf[8];
    int bad = 0;
    auto f = do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3;
    if (f(&self, 7, &self, buf, buf) != 6 || calls) { std::printf("same type\n"); ++bad; }
    if (f(&self, 7, &same_name, buf, buf) != 6 || calls) { std::printf("equal name\n"); ++bad; }
    if (f(&self, 7, &other, buf, buf) != 1234 || calls != 1 || last_offset != 7 || last_object != buf ||
        last_sub != buf || last_self != reinterpret_cast<const char*>(&base) + 16) { std::printf("forward on type mismatch\n"); ++bad; }
    calls = 0;
    if (f(&self, 3, &self, buf, buf + 2) != 1234 || calls != 1 || last_offset != 3 || last_sub != buf + 2) { std::printf("forward on pointer mismatch\n"); ++bad; }
    std::printf(bad ? "failed\n" : "si do_find_public_subobj cases passed\n");
    return bad != 0;
}
