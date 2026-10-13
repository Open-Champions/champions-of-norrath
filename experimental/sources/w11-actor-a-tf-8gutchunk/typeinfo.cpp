// Observed __tf8GutChunk at 0x0030fc10, 176 bytes (weak). The 12-byte typeinfo object __ti8GutChunk is zero in
// the file image and is defined here. Three nested levels (outer to inner) 8GutChunk > 6PhProp > 4Prop, base
// 10GameObject; only the first word of a node gates its level. When __ti8GutChunk is zero: if __ti6PhProp is
// zero, then if __ti4Prop is zero, __tf10GameObject runs and __rtti_si registers "4Prop" on __ti10GameObject;
// next (still inside the 6PhProp gate) "6PhProp" is registered on __ti4Prop; and last "8GutChunk" is
// registered on __ti6PhProp. A nonzero first word at a level skips that level's registration and the levels
// inside it. The object is returned either way. The nodes of 6PhProp and 4Prop are owned by other sources.
// Inferred: the chain of base classes, from the registration calls alone. Nothing here ran on the EE.
extern "C" {
unsigned char __ti8GutChunk[12] __attribute__((aligned(4)));
extern unsigned char __ti10GameObject[];
extern unsigned char __ti6PhProp[];
extern unsigned char __ti4Prop[];
void* __tf10GameObject();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf8GutChunk() {
    if (unset(__ti8GutChunk)) {
        if (unset(__ti6PhProp)) {
            if (unset(__ti4Prop)) {
                __tf10GameObject();
                __rtti_si(__ti4Prop, "4Prop", __ti10GameObject);
            }
            __rtti_si(__ti6PhProp, "6PhProp", __ti4Prop);
        }
        __rtti_si(__ti8GutChunk, "8GutChunk", __ti6PhProp);
    }
    return __ti8GutChunk;
}
