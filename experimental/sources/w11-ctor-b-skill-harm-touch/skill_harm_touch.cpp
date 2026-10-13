// __14SkillHarmTouch at 0x29aea8, 48 bytes: constructor of SkillHarmTouch, called with the address of a
// 104-byte skill object. Observed stores, all through the argument and nothing else
// (no calls, no reads, no return value): the address of the text "harmtouch" at +0,
// 0 at +4, 7 at +8, the halfword 300 at +12 and the address of _vt$14SkillHarmTouch (0x451100) at +96.
// The image's name text sits at 0x477fd0; this source uses its own copy of the text, so only the
// text is reproduced, not its address. Unknown: what the three numbers mean, and whether the
// name text is shared with any other table.
extern "C" {
extern unsigned char _vt$14SkillHarmTouch[];

namespace {
const char skill_name[] = "harmtouch";

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

void __14SkillHarmTouch(void* object) {
    SkillHead* skill = static_cast<SkillHead*>(object);
    skill->name = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(skill_name));
    skill->word4 = 0;
    skill->word8 = 7;
    skill->half12 = 300;
    skill->table = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$14SkillHarmTouch));
}
}
