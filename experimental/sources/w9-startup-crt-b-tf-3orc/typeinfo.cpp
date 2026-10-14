// Observed __tf3Orc at 0x0030f408, 80 bytes (weak).
// Returns the 12-byte type_info node __ti3Orc. Only the first word of a node gates
// initialization. When the outer node is zero:
//   if empty: call __tf8Creature, then __rtti_si(node, "3Orc", __ti8Creature).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti3Orc[12] __attribute__((aligned(4)));
extern unsigned char __ti8Creature[] __attribute__((aligned(4)));
void* __tf8Creature();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf3Orc() {
    if (unset(__ti3Orc)) {
        __tf8Creature();
        __rtti_si(__ti3Orc, "3Orc", __ti8Creature);
    }
    return __ti3Orc;
}
