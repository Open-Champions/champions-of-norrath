// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include "si_upcast.cpp"

static int calls, last_path;
static const void* last_self;
static void* last_object;
static NorrathUpcastResult* last_result;
extern "C" int __eq__C9type_infoRC9type_info(const void* l, const void* r) {
    return l == r || std::strcmp(*static_cast<const char* const*>(l), *static_cast<const char* const*>(r)) == 0;
}
static int forwarded(const void* self, int path, const void*, void* object, NorrathUpcastResult* result) {
    ++calls; last_self = self; last_path = path; last_object = object; last_result = result;
    return 99;
}

int main() {
    NorrathTypeInfoSlot table[6] = {};
    table[2].adjustment = 8;
    table[2].function = reinterpret_cast<void*>(forwarded);
    NorrathSiTypeInfo base = {"base", table, nullptr};
    NorrathSiTypeInfo self = {"abc", table, &base};
    NorrathSiTypeInfo same = {"abc", table, nullptr};
    NorrathSiTypeInfo other = {"xyz", table, nullptr};
    int bad = 0, object;
    for (int path = 0; path < 16; ++path) {
        NorrathUpcastResult res = {nullptr, -1, -1};
        int got = do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result(
            &self, path, path & 1 ? &self : &same, &object, &res);
        if (got != ((path & 6) == 4) || res.object != &object || res.access_path != path || res.marker != 8 || calls) { std::printf("match path %d\n", path); ++bad; }
        NorrathUpcastResult res2 = {nullptr, -1, -1};
        got = do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result(
            &self, path, &other, &object, &res2);
        if (got != 99 || calls != 1 || last_path != path || last_object != &object || last_result != &res2 ||
            last_self != reinterpret_cast<const char*>(&base) + 8 || res2.object != nullptr) { std::printf("forward path %d\n", path); ++bad; }
        calls = 0;
    }
    std::printf(bad ? "failed\n" : "si do_upcast cases passed\n");
    return bad != 0;
}
