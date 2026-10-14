// Observed __tf16SkillCriticalHit at 0x00318e88, 76 bytes (weak function in the image).
// Returns the 12-byte node __ti16SkillCriticalHit. When its first word is zero it calls __tf5Skill, and then calls
// __rtti_si with this node, the class-name text "16SkillCriticalHit" and the __ti5Skill node.
// A nonzero first word on this node skips everything. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Skill is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti16SkillCriticalHit[12] __attribute__((aligned(4)));
extern unsigned char __ti5Skill[8] __attribute__((aligned(4)));
void* __tf5Skill();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf16SkillCriticalHit() {
    if (unset(__ti16SkillCriticalHit)) {
        __tf5Skill();
        __rtti_si(__ti16SkillCriticalHit, "16SkillCriticalHit", __ti5Skill);
    }
    return __ti16SkillCriticalHit;
}
