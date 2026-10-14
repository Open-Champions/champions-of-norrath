// Host comparison only. Not an EE build input.
#include <cstdio>
extern "C" const unsigned char norrath_legacy_user_type_info_vtable_view[48] = {};
#include "rtti_user.cpp"
int main() {
    const void* slots[4] = {(void*)1, (void*)2, (void*)3, (void*)4};
    const char* name = "10GameObject";
    __rtti_user(slots, name);
    if (slots[0] != name || slots[1] != norrath_legacy_user_type_info_vtable_view) { std::puts("words 0/1 wrong"); return 1; }
    if (slots[2] != (void*)3 || slots[3] != (void*)4) { std::puts("must not touch later words"); return 2; }
    const void* other[2] = {(void*)7, (void*)8};
    __rtti_user(nullptr, name);
    if (other[0] != (void*)7 || other[1] != (void*)8) return 3;
    std::puts("rtti_user cases passed");
    return 0;
}
