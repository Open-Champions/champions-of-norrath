// Host check only. Not an EE build input.
#include <cstdio>
#include <cstring>

extern "C" void dtor_9type_info(void* object, int flags) asm("_$_9type_info");
extern "C" unsigned char norrath_rtti_entities_a_view_453850[8] = {};
static int deletes = 0;
static void* deleted;
extern "C" void __builtin_delete(void* object) { ++deletes; deleted = object; }

int main() {
    int bad = 0;
    const int flags_to_try[] = {0, 1, 2, 3, 4, -1, -2};
    for (int flags : flags_to_try) {
        const void* words[3] = {reinterpret_cast<const void*>(0x1234), nullptr, reinterpret_cast<const void*>(0x5678)};
        deletes = 0; deleted = nullptr;
        dtor_9type_info(words, flags);
        if (words[0] != reinterpret_cast<const void*>(0x1234) || words[2] != reinterpret_cast<const void*>(0x5678)) { std::printf("other words changed\n"); ++bad; }
        if (words[1] != norrath_rtti_entities_a_view_453850) { std::printf("table word flags %d\n", flags); ++bad; }
        if (deletes != ((flags & 1) ? 1 : 0) || ((flags & 1) && deleted != words)) { std::printf("delete flags %d\n", flags); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_$_9type_info cases passed\n");
    return bad != 0;
}
