#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
extern unsigned char __ti10AutoPropLP[12];
void* __tf10AutoPropLP();
}
extern "C" unsigned char __ti8AutoProp[12] = {};
extern "C" unsigned char __ti10AutoObject[12] = {};
extern "C" unsigned char __ti10GameObject[12] = {};
static int base_calls = 0;
extern "C" void* __tf10GameObject() { ++base_calls; return __ti10GameObject; }
struct Call { void* node; std::string name; const void* base; int base_calls_before; };
static std::vector<Call> calls;
extern "C" void __rtti_si(void* typeinfo, const char* name, const void* base) {
    calls.push_back({typeinfo, name, base, base_calls});
}
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void reset(bool lp, bool prop, bool object) {
    std::memset(__ti10AutoPropLP, 0, 12);
    std::memset(__ti8AutoProp, 0, 12);
    std::memset(__ti10AutoObject, 0, 12);
    if (lp) __ti10AutoPropLP[1] = 1;
    if (prop) __ti8AutoProp[2] = 1;
    if (object) __ti10AutoObject[3] = 1;
    calls.clear();
    base_calls = 0;
}
int main() {
    reset(false, false, false);
    expect(__tf10AutoPropLP() == __ti10AutoPropLP, "returns the node");
    expect(base_calls == 1, "GameObject accessor runs once");
    expect(calls.size() == 3, "three registrations");
    if (calls.size() == 3) {
        expect(calls[0].node == __ti10AutoObject && calls[0].name == "10AutoObject" &&
                   calls[0].base == __ti10GameObject && calls[0].base_calls_before == 1,
               "innermost registration after the base call");
        expect(calls[1].node == __ti8AutoProp && calls[1].name == "8AutoProp" && calls[1].base == __ti10AutoObject,
               "middle registration");
        expect(calls[2].node == __ti10AutoPropLP && calls[2].name == "10AutoPropLP" && calls[2].base == __ti8AutoProp,
               "outer registration");
    }
    reset(false, false, true);
    __tf10AutoPropLP();
    expect(base_calls == 0 && calls.size() == 2 && calls[0].name == "8AutoProp" && calls[1].name == "10AutoPropLP",
           "AutoObject already set");
    reset(false, true, false);
    __tf10AutoPropLP();
    expect(base_calls == 0 && calls.size() == 1 && calls[0].name == "10AutoPropLP" && calls[0].base == __ti8AutoProp,
           "AutoProp set skips AutoObject even when it is empty");
    reset(true, false, false);
    expect(__tf10AutoPropLP() == __ti10AutoPropLP && base_calls == 0 && calls.empty(), "outer set does nothing");
    reset(false, true, true);
    __tf10AutoPropLP();
    expect(base_calls == 0 && calls.size() == 1 && calls[0].name == "10AutoPropLP", "outer only");
    reset(false, false, false);
    __ti10AutoPropLP[0] = 0;
    __ti10AutoPropLP[4] = 7;
    __tf10AutoPropLP();
    expect(calls.size() == 3, "only the first word gates");
    return failures ? 1 : 0;
}
