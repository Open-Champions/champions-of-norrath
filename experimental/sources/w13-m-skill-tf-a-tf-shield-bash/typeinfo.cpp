// Observed __tf15SkillShieldBash at 0x0031a740, 76 bytes (weak function in the image).
// Returns the 12-byte node __ti15SkillShieldBash. When its first word is zero it calls __tf5Skill, and then calls
// __rtti_si with this node, the class-name text "15SkillShieldBash" and the __ti5Skill node.
// A nonzero first word on this node skips everything. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Skill is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti15SkillShieldBash[12] __attribute__((aligned(4)));
extern unsigned char __ti5Skill[8] __attribute__((aligned(4)));
void* __tf5Skill();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf15SkillShieldBash() {
    if (unset(__ti15SkillShieldBash)) {
        __tf5Skill();
        __rtti_si(__ti15SkillShieldBash, "15SkillShieldBash", __ti5Skill);
    }
    return __ti15SkillShieldBash;
}
