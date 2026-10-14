// Host contract for _$_10bad_typeid; not an EE input. The table is a local array standing in for _vt$9exception.
#include "dtor.cpp"
#include <cstdio>

extern "C" const unsigned char vt_9exception[32] = {};
static int deletes = 0;
static void* deleted;
extern "C" void __builtin_delete(void* object) { ++deletes; deleted = object; }

int main() {
    int bad = 0;
    const int flags_to_try[] = {0, 1, 2, 3, 4, -1, -2};
    for (int flags : flags_to_try) {
        const void* words[3] = {reinterpret_cast<const void*>(0x1234), reinterpret_cast<const void*>(0x9abc), reinterpret_cast<const void*>(0x5678)};
        deletes = 0; deleted = nullptr;
        dtor_10bad_typeid(words, flags);
        if (words[0] != static_cast<const void*>(vt_9exception)) { std::printf("table word flags %d\n", flags); ++bad; }
        if (words[1] != reinterpret_cast<const void*>(0x9abc) || words[2] != reinterpret_cast<const void*>(0x5678)) { std::printf("other words changed\n"); ++bad; }
        if (deletes != ((flags & 1) ? 1 : 0) || ((flags & 1) && deleted != words)) { std::printf("delete flags %d\n", flags); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_$_10bad_typeid cases passed\n");
    return bad != 0;
}
