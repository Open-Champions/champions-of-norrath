// Observed __tf16VIRealmInterface, 80 bytes at 0x012104f0 in the reference image. It returns the node __ti16VIRealmInterface (12 bytes).
// A nonzero first word returns the node at once. When that word is zero the function calls __tfQ212RealmService15RealmGatewayAPI, then registers
// "16VIRealmInterface" with __rtti_si against __tiQ212RealmService15RealmGatewayAPI.
// Inferred: single-base chain, from the same call shape as the other single-base getters.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti16VIRealmInterface[12] __attribute__((aligned(4)));
extern unsigned char __tiQ212RealmService15RealmGatewayAPI[8] __attribute__((aligned(4)));
void* __tfQ212RealmService15RealmGatewayAPI();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf16VIRealmInterface() {
    if (unset(__ti16VIRealmInterface)) {
        __tfQ212RealmService15RealmGatewayAPI();
        __rtti_si(__ti16VIRealmInterface, "16VIRealmInterface", __tiQ212RealmService15RealmGatewayAPI);
    }
    return __ti16VIRealmInterface;
}
