// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" unsigned char nodes[6096 * 20] = {};

struct Report { const char* file; int line; };
static std::vector<Report> reports;
static int faults = 0;
extern "C" int scePrintf(const char* format, ...) {
    // The only caller passes the fixed breach text, a file and a line.
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const char* file = __builtin_va_arg(args, const char*);
    int line = __builtin_va_arg(args, int);
    __builtin_va_end(args);
    if (std::strcmp(format, "list integrity breach! %s:%d\n") != 0) std::printf("unexpected format\n");
    reports.push_back({file, line});
    return 0;
}
extern "C" void norrath_list_check_host_fault_probe() { ++faults; }

#include "list_check.cpp"

static unsigned* words(int index) { return reinterpret_cast<unsigned*>(nodes) + index * 5; }
static void clear() { std::memset(nodes, 0, sizeof nodes); reports.clear(); faults = 0; }

static unsigned first_word() { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(nodes)); }

static int expect_failures(const char* name, size_t count) {
    reports.clear(); faults = 0;
    _listCheckAllIntegrity__FPCci("game.cc", 436);
    if (reports.size() != count || size_t(faults) != count) {
        std::printf("%s: expected %zu failures, got %zu reports and %d faults\n", name, count, reports.size(), faults);
        return 1;
    }
    for (const Report& r : reports) if (std::strcmp(r.file, "game.cc") != 0 || r.line != 436) { std::printf("%s: wrong arguments\n", name); return 1; }
    return 0;
}

int main() {
    int failures = 0;
    const unsigned first = first_word();
    const unsigned limit = first + 6096u * 20u;

    clear(); failures += expect_failures("all zero", 0);

    // Link words: inclusive range [first, limit].
    clear(); words(0)[0] = first; words(5)[2] = limit; words(6095)[0] = first + 20u; failures += expect_failures("links in range", 0);
    clear(); words(0)[0] = first - 1u; failures += expect_failures("link below table", 1);
    clear(); words(7)[2] = limit + 1u; failures += expect_failures("link above table", 1);
    clear(); words(100)[0] = 1u; words(100)[2] = 0xffffffffu; failures += expect_failures("two bad links in one record", 2);

    // Pointer words: [0x100000, 0x8000000] and [0x70000000, 0x70004000].
    const unsigned good[] = {0u, 0x00100000u, 0x08000000u, 0x70000000u, 0x70004000u, 0x00200000u};
    const unsigned bad[] = {0x000fffffu, 0x08000001u, 0x6fffffffu, 0x70004001u, 0xffffffffu, 1u, 0x0f000000u};
    for (unsigned w : good) { clear(); words(3)[1] = w; words(4)[3] = w; failures += expect_failures("good pointer", 0); }
    for (unsigned w : bad) {
        clear(); words(3)[1] = w; failures += expect_failures("bad pointer word 4", 1);
        clear(); words(6095)[3] = w; failures += expect_failures("bad pointer word 12 in last record", 1);
    }

    // Word 16 of a record is never checked; garbage there is fine.
    clear(); for (int i = 0; i < 6096; ++i) words(i)[4] = 0xdeadbeefu; failures += expect_failures("word 16 ignored", 0);

    // Every record is visited: one failure in each.
    clear(); for (int i = 0; i < 6096; ++i) words(i)[1] = 1u; failures += expect_failures("all records", 6096);

    if (failures) return 1;
    std::printf("list integrity cases passed\n");
    return 0;
}
