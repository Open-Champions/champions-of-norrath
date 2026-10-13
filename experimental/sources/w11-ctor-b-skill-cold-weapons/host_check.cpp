// Host contract for __16SkillColdWeapons; includes the source, not an EE input. Fails if any of the five stores
// is wrong or any other byte of the object (or the sentinel after it) is touched.
#include "skill_cold_weapons.cpp"
#include <cstdio>
#include <cstring>

unsigned char _vt$16SkillColdWeapons[144];

int main() {
    unsigned char buf[112];
    std::memset(buf, 0xa5, sizeof buf);
    __16SkillColdWeapons(buf);
    unsigned char want[112];
    std::memset(want, 0xa5, sizeof want);
    unsigned name = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(skill_name));
    unsigned table = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$16SkillColdWeapons));
    std::memcpy(want + 0, &name, 4);
    int w4 = 0, w8 = 0;
    short h = 30;
    std::memcpy(want + 4, &w4, 4);
    std::memcpy(want + 8, &w8, 4);
    std::memcpy(want + 12, &h, 2);
    std::memcpy(want + 96, &table, 4);
    int bad = 0;
    if (std::memcmp(buf, want, sizeof buf) != 0) { std::puts("object bytes differ"); bad = 1; }
    if (std::strcmp(skill_name, "coldweapons") != 0) { std::puts("name text differs"); bad = 1; }
    return bad;
}
