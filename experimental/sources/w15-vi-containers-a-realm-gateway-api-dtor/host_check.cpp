// Host check only; not a build input. The owned-object and table words are tags translated to host objects.
#define NORRATH_HOST_CONTRACT 1
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char vt_RealmGatewayAPI[0x240] __asm__("_vt$Q212RealmService15RealmGatewayAPI");
static unsigned object[8];
static unsigned char owned[160];
static VtEntry table[4];
static std::string trace;
static const unsigned TAG_OWNED = 0x1000, TAG_TABLE = 0x2000;
extern "C" char* norrath_host_translate(unsigned address) {
    switch (address) {
    case TAG_OWNED: return reinterpret_cast<char*>(owned);
    case TAG_TABLE: return reinterpret_cast<char*>(table);
    default: return nullptr;
    }
}
extern "C" void __builtin_delete(void* p) { trace += p == object ? "delete;" : "delete?;"; }
static void owned_dtor(void* self, int flags) {
    trace += "owned:" + std::to_string(static_cast<unsigned char*>(self) - owned) + "," + std::to_string(flags) + ";";
}
static void other_dtor(void*, int) { trace += "wrong-entry;"; }
int main() {
    bool ok = true;
    const unsigned vt = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_RealmGatewayAPI));
    table[0] = {0, 0, other_dtor};
    table[1] = {12, 0, owned_dtor};
    table[2] = {0, 0, other_dtor};
    for (unsigned has : {0u, 1u})
        for (int flags : {0, 1, 2, 3, -1, -2}) {
            trace.clear();
            std::memset(object, 0x5a, sizeof object);
            std::memset(owned, 0x5a, sizeof owned);
            object[0] = has ? TAG_OWNED : 0;
            *reinterpret_cast<unsigned*>(owned + 100) = TAG_TABLE;
            _$_Q212RealmService15RealmGatewayAPI(object, flags);
            std::string want;
            if (has) want += "owned:12,3;";
            if (flags & 1) want += "delete;";
            if (trace != want) ok = false;
            if (object[2] != vt || object[0] != (has ? TAG_OWNED : 0u)) ok = false;
            for (int i = 1; i < 8; ++i) if (i != 2 && object[i] != 0x5a5a5a5au) ok = false;
        }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
