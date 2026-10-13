// Observed __tf10SkillBlunt at 0x00313a58, pinned SLUS_205.65 (weak).
// Returns the 12-byte node __ti10SkillBlunt. When its first word is zero it first makes sure the 8-byte node __ti5Skill is set up, doing inline what
// __tf5Skill does (when that node's first word is zero, __rtti_user(node, "5Skill")), and then calls
// __rtti_si with this node, the class-name text "10SkillBlunt" and the __ti5Skill node.
// A nonzero first word on this node skips everything. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Skill is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti10SkillBlunt[12] __attribute__((aligned(4)));
extern unsigned char __ti5Skill[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10SkillBlunt() {
    if (unset(__ti10SkillBlunt)) {
        if (unset(__ti5Skill)) __rtti_user(__ti5Skill, "5Skill");
        __rtti_si(__ti10SkillBlunt, "10SkillBlunt", __ti5Skill);
    }
    return __ti10SkillBlunt;
}
