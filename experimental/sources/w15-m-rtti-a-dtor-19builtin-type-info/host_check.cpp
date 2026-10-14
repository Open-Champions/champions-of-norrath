// Host contract for _$_19__builtin_type_info; not an EE input. The table and base destructor are local stand-ins.
#include "dtor.cpp"
#include <cstdio>

extern "C" const unsigned char vt_19builtin_type_info[24] = {};
static int base_calls = 0;
static void* base_object;
static int base_flags = -1;
static int deletes = 0;
static void* deleted;
extern "C" void dtor_9type_info(void* object, int flags) { ++base_calls; base_object = object; base_flags = flags; }
extern "C" void __builtin_delete(void* object) { ++deletes; deleted = object; }

int main() {
    int bad = 0;
    const int flags_to_try[] = {0, 1, 2, 3, 4, -1, -2};
    for (int flags : flags_to_try) {
        const void* words[3] = {reinterpret_cast<const void*>(0x1234), reinterpret_cast<const void*>(0x9abc), reinterpret_cast<const void*>(0x5678)};
        base_calls = 0; base_object = nullptr; base_flags = -1;
        deletes = 0; deleted = nullptr;
        dtor_19builtin_type_info(words, flags);
        if (words[1] != static_cast<const void*>(vt_19builtin_type_info)) { std::printf("table word flags %d\n", flags); ++bad; }
        if (words[0] != reinterpret_cast<const void*>(0x1234) || words[2] != reinterpret_cast<const void*>(0x5678)) { std::printf("other words changed\n"); ++bad; }
        if (base_calls != 1 || base_object != words || base_flags != 0) { std::printf("base call flags %d\n", flags); ++bad; }
        if (deletes != ((flags & 1) ? 1 : 0) || ((flags & 1) && deleted != words)) { std::printf("delete flags %d\n", flags); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_$_19__builtin_type_info cases passed\n");
    return bad != 0;
}
