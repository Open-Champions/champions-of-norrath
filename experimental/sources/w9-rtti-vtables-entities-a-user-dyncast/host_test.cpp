// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include "user_dyncast.cpp"

extern "C" int __eq__C9type_infoRC9type_info(const void* l, const void* r) {
    return l == r || std::strcmp(*static_cast<const char* const*>(l), *static_cast<const char* const*>(r)) == 0;
}

int main() {
    const char* self[1] = {"A"};
    const char* same[1] = {"A"};
    const char* other[1] = {"B"};
    const void* types[3] = {self, same, other};
    int bad = 0;
    for (int tg = 0; tg < 3; ++tg)
        for (int st = 0; st < 3; ++st)
            for (int same_ptr = 0; same_ptr < 2; ++same_ptr)
                for (int path = 1; path < 8; path += 3) {
                    char buffer[16] = {};
                    NorrathDyncastResult res = {nullptr, -1, -1, -1};
                    int got = do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result(
                        self, 77, path, types[tg], buffer, types[st], same_ptr ? static_cast<void*>(buffer) : buffer + 4, &res);
                    if (got != 0) { std::printf("must return 0\n"); ++bad; }
                    bool first = same_ptr && st < 2;
                    bool second = tg < 2;
                    if (res.source_path != (first ? path : -1)) { std::printf("source path tg%d st%d\n", tg, st); ++bad; }
                    // when the first test applies the second one is not run
                    bool did_second = !first && second;
                    if ((res.object == buffer) != did_second || res.access_path != (did_second ? path : -1) ||
                        res.found != (did_second ? 1 : -1)) { std::printf("target fields tg%d st%d ptr%d\n", tg, st, same_ptr); ++bad; }
                }
    std::printf(bad ? "failed\n" : "user do_dyncast cases passed\n");
    return bad != 0;
}
