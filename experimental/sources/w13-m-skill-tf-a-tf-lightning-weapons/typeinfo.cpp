// Observed __tf21SkillLightningWeapons at 0x00318b10, 76 bytes (weak function in the image).
// Returns the 12-byte node __ti21SkillLightningWeapons. When its first word is zero it calls __tf5Skill, and then calls
// __rtti_si with this node, the class-name text "21SkillLightningWeapons" and the __ti5Skill node.
// A nonzero first word on this node skips everything. The node is returned either way.
// Inferred: the node is the class's runtime type descriptor and Skill is its single base.
// Nothing here was executed on the EE.
extern "C" {
unsigned char __ti21SkillLightningWeapons[12] __attribute__((aligned(4)));
extern unsigned char __ti5Skill[8] __attribute__((aligned(4)));
void* __tf5Skill();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf21SkillLightningWeapons() {
    if (unset(__ti21SkillLightningWeapons)) {
        __tf5Skill();
        __rtti_si(__ti21SkillLightningWeapons, "21SkillLightningWeapons", __ti5Skill);
    }
    return __ti21SkillLightningWeapons;
}
