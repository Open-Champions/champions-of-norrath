// Host check only; not an EE input. __tfQ212RealmService15RealmGatewayAPI and __rtti_si are recording stubs; the nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti16VIRealmInterface[12] __attribute__((aligned(4)));
unsigned char __tiQ212RealmService15RealmGatewayAPI[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void* __tfQ212RealmService15RealmGatewayAPI() { calls += "base;"; return __tiQ212RealmService15RealmGatewayAPI; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + (t == __ti16VIRealmInterface ? "node" : "?") + "," + name + "," + (base == __tiQ212RealmService15RealmGatewayAPI ? "RealmGatewayAPI" : "?") + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}

int main() {
    int failures = 0;
    const char* want = "base;si(node,16VIRealmInterface,RealmGatewayAPI);";
    // Zero first word: the base getter runs once, __rtti_si registers this node, and the node is returned.
    std::memset(__ti16VIRealmInterface, 0, sizeof __ti16VIRealmInterface);
    calls.clear();
    if (__tf16VIRealmInterface() != __ti16VIRealmInterface) { std::fprintf(stderr, "fail: zero word return\n"); ++failures; }
    if (calls != want) { std::fprintf(stderr, "fail: zero word calls %s\n", calls.c_str()); ++failures; }
    // Nonzero first word, with other bytes set: returned at once, no calls.
    const unsigned char set_words[3][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}};
    for (const auto& w : set_words) {
        std::memset(__ti16VIRealmInterface, 0x5a, sizeof __ti16VIRealmInterface);
        std::memcpy(__ti16VIRealmInterface, w, 4);
        calls.clear();
        if (__tf16VIRealmInterface() != __ti16VIRealmInterface) { std::fprintf(stderr, "fail: set word return\n"); ++failures; }
        if (!calls.empty()) { std::fprintf(stderr, "fail: set word calls %s\n", calls.c_str()); ++failures; }
    }
    if (!failures) std::puts("w16-m-data-a-tf-16virealminterface ok");
    return failures ? 1 : 0;
}
