// Host check only; not an EE build input.
#include <cstdio>
extern "C" void norrath_rtti_si(void* record, const char* name, void* base);
extern "C" const unsigned char norrath_legacy_single_inheritance_type_info_vtable_view[48] = {};

int main() {
    const void* slots[5] = {(void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
    int base_marker = 0;
    const char* name = "12SomeType";
    norrath_rtti_si(slots, name, &base_marker);
    if (slots[0] != name) return 1;
    if (slots[1] != norrath_legacy_single_inheritance_type_info_vtable_view) return 2;
    if (slots[2] != &base_marker) return 3;
    if (slots[3] != (void*)4 || slots[4] != (void*)5) return 4;  // nothing past +8
    norrath_rtti_si(nullptr, name, &base_marker);                  // null record is a no-op
    std::puts("rtti_si ok");
    return 0;
}
