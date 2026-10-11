#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
extern unsigned char __ti13PokeReflector[12];
void* __tf13PokeReflector();
}
static int base_calls = 0;
extern "C" unsigned char norrath_ti_10GameObject[8] = {};
extern "C" void* norrath_vt_tf_10GameObject() { ++base_calls; return norrath_ti_10GameObject; }

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
    std::memset(__ti13PokeReflector, 0, 12);
    expect(__tf13PokeReflector() == __ti13PokeReflector, "returns the type-info object");
    expect(base_calls == 1, "base type info runs first");
    expect(rtti_calls == 1 && seen_ti == __ti13PokeReflector, "registers this object");
    expect(seen_name == "13PokeReflector", "class name");
    expect(seen_base == norrath_ti_10GameObject, "base object");
    __ti13PokeReflector[0] = 1;
    expect(__tf13PokeReflector() == __ti13PokeReflector, "second return");
    expect(base_calls == 1 && rtti_calls == 1, "nonzero word skips registration");
    return failures ? 1 : 0;
}
