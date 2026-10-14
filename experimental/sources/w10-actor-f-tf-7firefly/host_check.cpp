#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
extern unsigned char __ti7FireFly[12];
void* __tf7FireFly();
}
extern "C" unsigned char __ti10FireBeetle[12] = {};
extern "C" unsigned char __ti8Creature[12] = {};
static int base_calls = 0;
extern "C" void* __tf8Creature() { ++base_calls; return __ti8Creature; }
struct Call { void* ti; std::string name; void* base; };
static std::vector<Call> calls;
static int failures = 0;
extern "C" void __rtti_si(void* typeinfo, const char* name, void* base) { calls.push_back({typeinfo, name, base}); }
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    std::memset(__ti7FireFly, 0, 12);
    std::memset(__ti10FireBeetle, 0, 12);
    expect(__tf7FireFly() == __ti7FireFly, "returns the type-info object");
    expect(base_calls == 1 && calls.size() == 2, "both levels register");
    expect(calls[0].ti == __ti10FireBeetle && calls[0].name == "10FireBeetle" && calls[0].base == __ti8Creature, "FireBeetle first");
    expect(calls[1].ti == __ti7FireFly && calls[1].name == "7FireFly" && calls[1].base == __ti10FireBeetle, "FireFly second");
    // FireBeetle already registered
    std::memset(__ti7FireFly, 0, 12);
    __ti10FireBeetle[0] = 7;
    base_calls = 0; calls.clear();
    expect(__tf7FireFly() == __ti7FireFly, "return with registered base");
    expect(base_calls == 0 && calls.size() == 1 && calls[0].ti == __ti7FireFly && calls[0].base == __ti10FireBeetle, "only FireFly registers");
    // FireFly already registered
    __ti7FireFly[0] = 1;
    base_calls = 0; calls.clear();
    __ti10FireBeetle[0] = 0;
    expect(__tf7FireFly() == __ti7FireFly, "return when registered");
    expect(base_calls == 0 && calls.empty(), "nonzero word skips everything");
    return failures ? 1 : 0;
}
