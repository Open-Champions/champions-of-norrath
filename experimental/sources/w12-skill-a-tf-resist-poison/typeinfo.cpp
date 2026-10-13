// Observed __tf17SkillResistPoison at 0x00313c38, pinned SLUS_205.65 (weak).
// Returns the 12-byte node __ti17SkillResistPoison. When its first word is zero it first makes sure the 8-byte node __ti5Skill is set up, doing inline what
// __tf5Skill does (when that node's first word is zero, __rtti_user(node, "5Skill")), and then calls
// __rtti_si with this node, the class-name text "17SkillResistPoison" and the __ti5Skill node.
// A nonzero first word on this node skips everything. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Skill is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti17SkillResistPoison[12] __attribute__((aligned(4)));
extern unsigned char __ti5Skill[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf17SkillResistPoison() {
    if (unset(__ti17SkillResistPoison)) {
        if (unset(__ti5Skill)) __rtti_user(__ti5Skill, "5Skill");
        __rtti_si(__ti17SkillResistPoison, "17SkillResistPoison", __ti5Skill);
    }
    return __ti17SkillResistPoison;
}
