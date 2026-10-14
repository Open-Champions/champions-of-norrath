// Observed __tf5Skill at 0x003139c8, 60 bytes (weak), and its node __ti5Skill, an 8-byte zeroed object in
// the image's small uninitialised data (0x00486dd0).
// Returns the node. When its first word is zero it calls __rtti_user(node, "5Skill"), which stores the name
// pointer in word 0 and the user-type-info table pointer in word 1. A nonzero first word skips the call.
// Inferred: Skill is a class with no base, so its descriptor is a plain user type node (8 bytes, not 12).
// Not authored here: __rtti_user.
extern "C" {
unsigned char __ti5Skill[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf5Skill() {
    if (unset(__ti5Skill)) {
        __rtti_user(__ti5Skill, "5Skill");
    }
    return __ti5Skill;
}
