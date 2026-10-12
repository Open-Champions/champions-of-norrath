// Host comparison only. Not an EE build input.
#include <cstdio>

extern "C" const unsigned char norrath_legacy_single_inheritance_type_info_vtable_view[48] = {};
#include "rtti_si.cpp"

int main() {
    void* words[3] = {reinterpret_cast<void*>(1), reinterpret_cast<void*>(2), reinterpret_cast<void*>(3)};
    static char base_object[12];
    __rtti_si(nullptr, "ignored", base_object);
    if (words[0] != reinterpret_cast<void*>(1) || words[1] != reinterpret_cast<void*>(2)) { std::printf("null must not store\n"); return 1; }
    static const char name[] = "12CosmeticProp";
    __rtti_si(words, name, base_object);
    if (words[0] != name) { std::printf("name word wrong\n"); return 2; }
    if (words[1] != norrath_legacy_single_inheritance_type_info_vtable_view) { std::printf("table word wrong\n"); return 3; }
    if (words[2] != base_object) { std::printf("base word wrong\n"); return 4; }
    __rtti_si(words, nullptr, nullptr);
    if (words[0] != nullptr || words[2] != nullptr || words[1] != norrath_legacy_single_inheritance_type_info_vtable_view) { std::printf("null arguments must be stored\n"); return 5; }
    std::printf("__rtti_si cases passed\n");
    return 0;
}
