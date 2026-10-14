// Host check only. Not an EE build input.
#include <cstdio>
extern "C" void norrath_rtti_ptr(void* object, const char* name, void* target);
extern "C" unsigned char norrath_legacy_pointer_type_info_vtable_view[24] = {};
int main() {
    int bad = 0;
    const void* words[4] = {reinterpret_cast<const void*>(1), reinterpret_cast<const void*>(2),
                            reinterpret_cast<const void*>(3), reinterpret_cast<const void*>(0x77)};
    char name[] = "Px";
    int target = 0;
    norrath_rtti_ptr(words, name, &target);
    if (words[0] != name) { std::printf("name word\n"); ++bad; }
    if (words[1] != norrath_legacy_pointer_type_info_vtable_view) { std::printf("table word\n"); ++bad; }
    if (words[2] != &target) { std::printf("target word\n"); ++bad; }
    if (words[3] != reinterpret_cast<const void*>(0x77)) { std::printf("spill\n"); ++bad; }
    norrath_rtti_ptr(nullptr, name, &target);
    std::printf(bad ? "failed\n" : "__rtti_ptr cases passed\n");
    return bad != 0;
}
