#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
extern unsigned char __ti8Froglock[12];
void* __tf8Froglock();
}
static int base_calls = 0;
extern "C" unsigned char __ti8Creature[12] = {};
extern "C" void* __tf8Creature() { ++base_calls; return __ti8Creature; }
static int rtti_calls = 0;
static void* seen_ti = nullptr;
static std::string seen_name;
static void* seen_base = nullptr;
static int failures = 0;
extern "C" void __rtti_si(void* typeinfo, const char* name, void* base) {
    ++rtti_calls;
    seen_ti = typeinfo;
    seen_name = name;
    seen_base = base;
}
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    std::memset(__ti8Froglock, 0, 12);
    expect(__tf8Froglock() == __ti8Froglock, "returns the type-info object");
    expect(base_calls == 1, "base type info runs first");
    expect(rtti_calls == 1 && seen_ti == __ti8Froglock, "registers this object");
    expect(seen_name == "8Froglock", "class name");
    expect(seen_base == __ti8Creature, "base object");
    __ti8Froglock[0] = 1;
    expect(__tf8Froglock() == __ti8Froglock, "second return");
    expect(base_calls == 1 && rtti_calls == 1, "nonzero word skips registration");
    __ti8Froglock[0] = 0; __ti8Froglock[8] = 5;
    expect(__tf8Froglock() == __ti8Froglock && rtti_calls == 2 && base_calls == 2, "only the first word decides");
    return failures ? 1 : 0;
}
