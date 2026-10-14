// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include "user_upcast.cpp"

extern "C" int __eq__C9type_infoRC9type_info(const void* l, const void* r) {
    return l == r || std::strcmp(*static_cast<const char* const*>(l), *static_cast<const char* const*>(r)) == 0;
}

int main() {
    const char* self_name = "6Candle";
    const char* self[1] = {self_name};
    const char* same_name[1] = {"6Candle"};
    const char* other[1] = {"4Line"};
    int bad = 0;
    for (int path = 0; path < 16; ++path) {
        const void* targets[3] = {self, same_name, other};
        for (int t = 0; t < 3; ++t) {
            int object = 0;
            NorrathUpcastResult res = {nullptr, -1, -1};
            int got = do_upcast__C16__user_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result(
                self, path, targets[t], &object, &res);
            bool match = t < 2;
            int want = match && (path & 6) == 4;
            if (got != want) { std::printf("result path %d target %d\n", path, t); ++bad; }
            if (match && (res.object != &object || res.access_path != path || res.marker != 8)) { std::printf("fields path %d\n", path); ++bad; }
            if (!match && (res.object != nullptr || res.access_path != -1 || res.marker != -1)) { std::printf("mismatch stored\n"); ++bad; }
        }
    }
    std::printf(bad ? "failed\n" : "user do_upcast cases passed\n");
    return bad != 0;
}
