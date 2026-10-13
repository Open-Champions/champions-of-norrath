// Observed __tf8AntQueen at 0x30e968, 76 bytes (weak).
// Returns the 12-byte type_info node __ti8AntQueen (defined here: nothing else in the tree owns it).
// Only the first word of a node gates initialization. When the node is zero: call __tf8Creature, then __rtti_si(node, "8AntQueen", __ti8Creature).
// A nonzero first word at a level skips that level's work and the call below it;
// levels above it still run. The node is returned either way. __rtti_si takes
// the node, the class-name text and the base node.
// Inferred: the node is the class's runtime type descriptor, from the symbol names.
extern "C" {
unsigned char __ti8AntQueen[12] __attribute__((aligned(4)));
extern unsigned char __ti8Creature[] __attribute__((aligned(4)));
void* __tf8Creature();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf8AntQueen() {
    if (unset(__ti8AntQueen)) {
        __tf8Creature();
        __rtti_si(__ti8AntQueen, "8AntQueen", __ti8Creature);
    }
    return __ti8AntQueen;
}
