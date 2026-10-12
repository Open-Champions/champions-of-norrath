// Host check only; not an EE build input.
#include <cstdio>
extern "C" void norrath_rtti_user(void* record, const char* name);
extern "C" const unsigned char norrath_legacy_user_type_info_vtable_view[48] = {};

int main() {
    const void* slots[4] = {(void*)1, (void*)2, (void*)3, (void*)4};
    const char* name = "10GameObject";
    norrath_rtti_user(slots, name);
    if (slots[0] != name) return 1;
    if (slots[1] != norrath_legacy_user_type_info_vtable_view) return 2;
    if (slots[2] != (void*)3 || slots[3] != (void*)4) return 3;
    norrath_rtti_user(nullptr, name);
    std::puts("rtti_user ok");
    return 0;
}
