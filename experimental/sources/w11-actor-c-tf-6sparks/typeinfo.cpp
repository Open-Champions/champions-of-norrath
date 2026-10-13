// Observed __tf6Sparks at 0x00310b80, 132 bytes (weak).
// Returns the 12-byte type_info node __ti6Sparks (zero-initialized BSS). When its first word is zero:
// when the first word of the node __ti10Projectile is zero, __tf10GameObject is called and then
// __rtti_si sets up __ti10Projectile with the text "10Projectile" and the __ti10GameObject node (this
// is the Projectile node being initialized inline, as __tf10Projectile would do); then, in every case,
// __rtti_si is called with __ti6Sparks, the text "6Sparks" and the __ti10Projectile node. A nonzero
// first word on the Sparks node skips everything. The Sparks node is returned.
// Inferred: Sparks derives from Projectile, which derives from GameObject. Nothing here ran on the EE.
extern "C" {
unsigned char __ti6Sparks[12] __attribute__((aligned(4)));
extern unsigned char __ti10Projectile[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf6Sparks() {
    if (unset(__ti6Sparks)) {
        if (unset(__ti10Projectile)) {
            __tf10GameObject();
            __rtti_si(__ti10Projectile, "10Projectile", __ti10GameObject);
        }
        __rtti_si(__ti6Sparks, "6Sparks", __ti10Projectile);
    }
    return __ti6Sparks;
}
