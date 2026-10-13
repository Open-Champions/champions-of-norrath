// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include "si_dyncast.cpp"

static int calls, last_offset, last_path;
static const void* last_self;
static const void* last_source;
static void* last_sub;
static NorrathDyncastResult* last_result;
extern "C" int __eq__C9type_infoRC9type_info(const void* l, const void* r) {
    return l == r || std::strcmp(*static_cast<const char* const*>(l), *static_cast<const char* const*>(r)) == 0;
}
static int forwarded(const void* self, int offset, int path, const void*, void*, const void* source, void* sub,
                     NorrathDyncastResult* result) {
    ++calls; last_self = self; last_offset = offset; last_path = path; last_source = source; last_sub = sub; last_result = result;
    return 55;
}

int main() {
    NorrathTypeInfoSlot table[6] = {};
    table[3].adjustment = 4;
    table[3].function = reinterpret_cast<void*>(forwarded);
    NorrathSiTypeInfo base = {"base", table, nullptr};
    NorrathSiTypeInfo self = {"abc", table, &base};
    NorrathSiTypeInfo same = {"abc", table, nullptr};
    NorrathSiTypeInfo other = {"xyz", table, nullptr};
    const NorrathSiTypeInfo* types[3] = {&self, &same, &other};
    char buf[32];
    int bad = 0;
    auto f = do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result;
    for (int tg = 0; tg < 3; ++tg)
        for (int st = 0; st < 3; ++st)
            for (int delta = -8; delta <= 12; delta += 4)
                for (int offset = -3; offset <= 12; ++offset) {
                    NorrathDyncastResult res = {nullptr, -1, -1, -1};
                    calls = 0;
                    int got = f(&self, offset, 5, types[tg], buf + 8, types[st], buf + 8 + delta, &res);
                    bool first = delta == 0 && st < 2;
                    bool second = !first && tg < 2;
                    if (first) {
                        if (got != 0 || res.source_path != 5 || res.object || res.found != -1 || calls) { std::printf("first tg%d st%d\n", tg, st); ++bad; }
                    } else if (second) {
                        int want = -1;
                        if (offset >= 0) want = delta == offset ? 6 : 1;
                        else if (offset == -2) want = 1;
                        if (got != 0 || res.object != buf + 8 || res.access_path != 5 || res.found != want || res.source_path != -1 || calls) {
                            std::printf("second tg%d st%d delta%d off%d\n", tg, st, delta, offset); ++bad;
                        }
                    } else if (got != 55 || calls != 1 || last_offset != offset || last_path != 5 || last_source != types[st] ||
                               last_sub != buf + 8 + delta || last_result != &res ||
                               last_self != reinterpret_cast<const char*>(&base) + 4) {
                        std::printf("forward tg%d st%d\n", tg, st); ++bad;
                    }
                }
    std::printf(bad ? "failed\n" : "si do_dyncast cases passed\n");
    return bad != 0;
}
