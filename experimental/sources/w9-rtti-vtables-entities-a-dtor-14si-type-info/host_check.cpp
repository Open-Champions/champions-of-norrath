// Host check only. Not an EE build input.
#include <cstdio>
#include <cstring>

extern "C" void dtor_14__si_type_info(void* object, int flags) asm("_$_14__si_type_info");
extern "C" unsigned char norrath_legacy_user_type_info_vtable_view[8] = {};
extern "C" void type_info_destructor(void* object, int flags) asm("_$_9type_info");
static int deletes = 0, base_calls = 0, base_flags = 99, order = 0, base_order = 0, delete_order = 0;
static void* deleted;
static void* base_object;
static const void* table_at_base;
static const void* current_words;
extern "C" void type_info_destructor(void* object, int flags) {
    ++base_calls; base_flags = flags; base_object = object; base_order = ++order;
    table_at_base = static_cast<const void* const*>(object)[1];
}
extern "C" void __builtin_delete(void* object) { ++deletes; deleted = object; delete_order = ++order; }

int main() {
    int bad = 0;
    const int flags_to_try[] = {0, 1, 2, 3, 4, -1, -2};
    for (int flags : flags_to_try) {
        const void* words[3] = {reinterpret_cast<const void*>(0x1234), nullptr, reinterpret_cast<const void*>(0x5678)};
        deletes = base_calls = order = 0; base_flags = 99; deleted = base_object = nullptr; table_at_base = nullptr;
        dtor_14__si_type_info(words, flags);
        if (words[0] != reinterpret_cast<const void*>(0x1234) || words[2] != reinterpret_cast<const void*>(0x5678)) { std::printf("other words changed\n"); ++bad; }
        if (words[1] != norrath_legacy_user_type_info_vtable_view || table_at_base != norrath_legacy_user_type_info_vtable_view) { std::printf("table word flags %d\n", flags); ++bad; }
        if (base_calls != 1 || base_flags != 0 || base_object != words) { std::printf("base call flags %d\n", flags); ++bad; }
        if (deletes != ((flags & 1) ? 1 : 0) || ((flags & 1) && (deleted != words || delete_order < base_order))) { std::printf("delete flags %d\n", flags); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_$_14__si_type_info cases passed\n");
    return bad != 0;
}
