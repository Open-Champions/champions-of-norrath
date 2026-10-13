// Observed isEnemy__8SkeletonP10GameObject at 0x00247160, 240 bytes in SLUS_205.65.
// Arguments: the Skeleton and another object. Starts from the Creature base's answer,
// isEnemy__8CreatureP10GameObject: a nonzero base answer becomes that answer plus one (an
// unchanged zero stays zero). A null other object or a nonzero value ends the call with that
// value. Otherwise the other object's runtime type is read by calling the first function of
// the table referenced by its word at +72 (no argument) and compared with __tf8Skeleton()
// through __eq__C9type_infoRC9type_info. A different type, a set 0x08 bit in the other
// object's flag word (+12), or a nonzero halfword at +792 of the Skeleton leaves the answer 0.
// Else the answer is 1 exactly when the signed bytes at +972 of the two objects are
// {2 and 1} or {1 and 2} (this one and the other, either way round), and 0 otherwise.
// Inferred: byte +972 is a side or team marker; the +72 table word is the virtual table and
// byte offset 4 of it the type-descriptor slot. Unknown: the meanings of +792, the marker
// values and flag bit 0x08.
extern "C" int isEnemy__8CreatureP10GameObject(unsigned char* self, unsigned char* other);
extern "C" void* __tf8Skeleton();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);

extern "C" int isEnemy__8SkeletonP10GameObject(unsigned char* self, unsigned char* other) {
    int base = isEnemy__8CreatureP10GameObject(self, other);
    int result = 0;
    if (base != 0) result = base + 1;
    if (other == 0) return result;
    if (result != 0) return result;
    unsigned char* table = *reinterpret_cast<unsigned char**>(other + 72);
    void* type = (*reinterpret_cast<void* (**)()>(table + 4))();
    if (__eq__C9type_infoRC9type_info(type, __tf8Skeleton()) == 0) return result;
    if ((*reinterpret_cast<unsigned*>(other + 12) & 0x8) != 0) return result;
    if (*reinterpret_cast<short*>(self + 792) != 0) return result;
    const signed char mine = static_cast<signed char>(self[972]);
    const signed char theirs = static_cast<signed char>(other[972]);
    if (mine == 2) {
        if (theirs == 1) result = 1;
    } else if (mine == 1) {
        if (theirs == 2) result = 1;
    }
    return result;
}
