// __15SkillFireFlares at 0x2a4858, 44 bytes: constructor of SkillFireFlares, called with the address of a
// 104-byte skill object. Observed stores, all through the argument and nothing else
// (no calls, no reads, no return value): the address of the text "fireflares" at +0,
// 0 at +4, 0 at +8, the halfword 30 at +12 and the address of _vt$15SkillFireFlares (0x451a30) at +96.
// The image's name text sits at 0x478600; this source uses its own copy of the text, so only the
// text is reproduced, not its address. Unknown: what the three numbers mean, and whether the
// name text is shared with any other table.
extern "C" {
extern unsigned char _vt$15SkillFireFlares[];

namespace {
const char skill_name[] = "fireflares";

struct SkillHead {
    unsigned name;
    int word4;
    int word8;
    short half12;
    unsigned char gap[82];
    unsigned table;
};
static_assert(__builtin_offsetof(SkillHead, word8) == 8 && __builtin_offsetof(SkillHead, half12) == 12 &&
              __builtin_offsetof(SkillHead, table) == 96, "skill object offsets");
}  // namespace

void __15SkillFireFlares(void* object) {
    SkillHead* skill = static_cast<SkillHead*>(object);
    skill->name = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(skill_name));
    skill->word4 = 0;
    skill->word8 = 0;
    skill->half12 = 30;
    skill->table = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$15SkillFireFlares));
}
}
