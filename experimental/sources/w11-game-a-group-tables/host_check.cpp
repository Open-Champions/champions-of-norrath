// Host contract for the group table storage; not an EE input.
#include "group_tables.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof nmEqGroups == 4 && sizeof nmWGroups == 4 && sizeof eqGroup == 4 && sizeof wGroup == 4, "word storage");
    int bad = 0;
    if (nmEqGroups != 0 || nmWGroups != 0 || eqGroup != 0 || wGroup != 0) { std::puts("fail: initial values"); bad = 1; }
    nmEqGroups = 3; eqGroup = 0x1234;
    if (nmWGroups != 0 || wGroup != 0) { std::puts("fail: separate storage"); bad = 1; }
    if (!bad) std::puts("group tables ok");
    return bad;
}
