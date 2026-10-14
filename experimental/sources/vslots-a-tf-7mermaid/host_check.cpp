#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
extern unsigned char __ti7Mermaid[12];
void* __tf7Mermaid();
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
    std::memset(__ti7Mermaid, 0, 12);
    expect(__tf7Mermaid() == __ti7Mermaid, "returns the type-info object");
    expect(base_calls == 1, "base type info runs first");
    expect(rtti_calls == 1 && seen_ti == __ti7Mermaid, "registers this object");
    expect(seen_name == "7Mermaid", "class name");
    expect(seen_base == __ti8Creature, "base object");
    __ti7Mermaid[0] = 1;
    expect(__tf7Mermaid() == __ti7Mermaid, "second return");
    expect(base_calls == 1 && rtti_calls == 1, "nonzero word skips registration");
    return failures ? 1 : 0;
}
