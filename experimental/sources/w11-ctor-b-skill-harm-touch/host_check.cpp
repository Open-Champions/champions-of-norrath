// Host contract for __14SkillHarmTouch; includes the source, not an EE input. Fails if any of the five stores
// is wrong or any other byte of the object (or the sentinel after it) is touched.
#include "skill_harm_touch.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$14SkillHarmTouch[144];

int main() {
    unsigned char buf[112];
    std::memset(buf, 0xa5, sizeof buf);
    __14SkillHarmTouch(buf);
    unsigned char want[112];
    std::memset(want, 0xa5, sizeof want);
    unsigned name = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(skill_name));
    unsigned table = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$14SkillHarmTouch));
    std::memcpy(want + 0, &name, 4);
    int w4 = 0, w8 = 7;
    short h = 300;
    std::memcpy(want + 4, &w4, 4);
    std::memcpy(want + 8, &w8, 4);
    std::memcpy(want + 12, &h, 2);
    std::memcpy(want + 96, &table, 4);
    int bad = 0;
    if (std::memcmp(buf, want, sizeof buf) != 0) { std::puts("object bytes differ"); bad = 1; }
    if (std::strcmp(skill_name, "harmtouch") != 0) { std::puts("name text differs"); bad = 1; }
    return bad;
}
