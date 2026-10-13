// Observed __tf10GameObject at 0x00312dd0, 60 bytes (weak).
// Returns __ti10GameObject (0x00486dc8, an 8-byte node reached through gp). When
// its first word is zero it calls __rtti_user (0x002fd368) with the node and the
// class-name text "10GameObject"; a nonzero first word skips the call. The node
// is returned either way. Unlike the derived-class accessors there is no base
// call. Inferred: the node is the root type descriptor of the game-object
// classes, from every derived accessor using it as its base.
// Nothing here was executed on the EE.
extern "C" {
extern unsigned char __ti10GameObject[] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;

extern "C" void* __tf10GameObject() {
    if (*reinterpret_cast<const AliasedWord*>(__ti10GameObject) == 0u)
        __rtti_user(__ti10GameObject, "10GameObject");
    return __ti10GameObject;
}
