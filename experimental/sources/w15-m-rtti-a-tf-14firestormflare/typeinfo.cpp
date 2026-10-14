// Observed __tf14FireStormFlare at 0x00317a28, 76 bytes. It returns the node __ti14FireStormFlare (0x008eb898).
// A nonzero first word returns the node at once. When that word is zero the function calls __tf10GameObject,
// then registers "14FireStormFlare" with __rtti_si against the base node __ti10GameObject (0x00486dc8).
// Inferred: single inheritance from GameObject, from the registration call shape. Unknown: the name string
// contents beyond the class name; the base node address is taken from the image symbol of the same name.
extern "C" {
extern unsigned char __ti14FireStormFlare[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf14FireStormFlare() {
    if (unset(__ti14FireStormFlare)) {
        __tf10GameObject();
        __rtti_si(__ti14FireStormFlare, "14FireStormFlare", __ti10GameObject);
    }
    return __ti14FireStormFlare;
}
