#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
extern unsigned char __ti10Teleporter[12];
void* __tf10Teleporter();
}
static int base_calls = 0;
extern "C" unsigned char __ti8Creature[12] = {};
extern "C" void* __tf8Creature() { ++base_calls; return __ti8Creature; }
static int rtti_calls = 0;
static int order_ok = 1;
static void* seen_ti = nullptr;
static std::string seen_name;
static void* seen_base = nullptr;
static int failures = 0;
extern "C" void __rtti_si(void* typeinfo, const char* name, void* base) {
    if (base_calls != 1) order_ok = 0;
    ++rtti_calls;
    seen_ti = typeinfo;
    seen_name = name;
    seen_base = base;
}
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    std::memset(__ti10Teleporter, 0, 12);
    expect(__tf10Teleporter() == __ti10Teleporter, "returns the type-info object");
    expect(base_calls == 1, "base type info runs first");
    expect(order_ok == 1, "registration happens after the base call");
    expect(rtti_calls == 1 && seen_ti == __ti10Teleporter, "registers this object");
    expect(seen_name == "10Teleporter", "class name");
    expect(seen_base == __ti8Creature, "base object");
    __ti10Teleporter[0] = 1;
    expect(__tf10Teleporter() == __ti10Teleporter, "second return");
    expect(base_calls == 1 && rtti_calls == 1, "nonzero first byte skips registration");
    base_calls = rtti_calls = 0;
    std::memset(__ti10Teleporter, 0, 12);
    __ti10Teleporter[3] = 0x80;
    expect(__tf10Teleporter() == __ti10Teleporter && base_calls == 0 && rtti_calls == 0, "any byte of the first word gates");
    std::memset(__ti10Teleporter, 0, 12);
    __ti10Teleporter[4] = 9;
    __ti10Teleporter[11] = 9;
    expect(__tf10Teleporter() == __ti10Teleporter && base_calls == 1 && rtti_calls == 1, "only the first word gates");
    return failures ? 1 : 0;
}
