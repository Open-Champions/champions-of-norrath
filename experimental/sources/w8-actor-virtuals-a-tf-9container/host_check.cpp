#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
extern unsigned char __ti9Container[12];
void* __tf9Container();
}
static int base_calls = 0;
extern "C" unsigned char norrath_ti_10GameObject[12] = {};
extern "C" void* norrath_vt_tf_10GameObject() { ++base_calls; return norrath_ti_10GameObject; }
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
    std::memset(__ti9Container, 0, 12);
    expect(__tf9Container() == __ti9Container, "returns the type-info object");
    expect(base_calls == 1, "base type info runs first");
    expect(order_ok == 1, "registration happens after the base call");
    expect(rtti_calls == 1 && seen_ti == __ti9Container, "registers this object");
    expect(seen_name == "9Container", "class name");
    expect(seen_base == norrath_ti_10GameObject, "base object");
    __ti9Container[0] = 1;
    expect(__tf9Container() == __ti9Container, "second return");
    expect(base_calls == 1 && rtti_calls == 1, "nonzero first byte skips registration");
    base_calls = rtti_calls = 0;
    std::memset(__ti9Container, 0, 12);
    __ti9Container[3] = 0x80;
    expect(__tf9Container() == __ti9Container && base_calls == 0 && rtti_calls == 0, "any byte of the first word gates");
    std::memset(__ti9Container, 0, 12);
    __ti9Container[4] = 9;
    __ti9Container[11] = 9;
    expect(__tf9Container() == __ti9Container && base_calls == 1 && rtti_calls == 1, "only the first word gates");
    return failures ? 1 : 0;
}
