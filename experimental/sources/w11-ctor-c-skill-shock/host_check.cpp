// Host contract for __10SkillShock; not an EE input. Includes the source.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>

unsigned char vt_SkillShock[16];

int main() {
    int bad = 0;
    unsigned char raw[sizeof(SkillObject) + 8];
    std::memset(raw, 0xa5, sizeof raw);
    SkillObject* o = reinterpret_cast<SkillObject*>(raw);
    __10SkillShock(o);
    if (std::strcmp(o->name, "shock") != 0) { std::puts("fail: name"); bad = 1; }
    if (o->zero != 0) { std::puts("fail: +4"); bad = 1; }
    if (o->value != 0) { std::puts("fail: +8"); bad = 1; }
    if (o->half != 30) { std::puts("fail: +12"); bad = 1; }
    if (o->table != vt_SkillShock) { std::puts("fail: table"); bad = 1; }
    for (unsigned i = 0; i < sizeof(o->untouched); ++i)
        if (o->untouched[i] != (char)0xa5) { std::puts("fail: untouched byte written"); bad = 1; break; }
    for (unsigned i = sizeof(SkillObject); i < sizeof raw; ++i)
        if (raw[i] != 0xa5) { std::puts("fail: tail written"); bad = 1; break; }
    return bad;
}
