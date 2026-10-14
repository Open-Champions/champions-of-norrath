#include "clear.cpp"

static void* freed;
static int calls;
extern "C" void __builtin_vec_delete(void* pointer) { freed = pointer; ++calls; }

int main() {
    unsigned int words[6];
    for (int i = 0; i < 6; ++i) words[i] = 0x2000 + i;
    words[2] = 0x1c0;
    int result = Clear__14VIQueryTriList(words);
    if (result != 0 || calls != 1) return 1;
    if (freed != reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(0x1c0))) return 2;
    if (words[0] != 0 || words[1] != 0 || words[2] != 0) return 3;
    if (words[3] != 0x2003 || words[4] != 0x2004 || words[5] != 0x2005) return 4;
    // a null buffer is not deleted but the counters are still cleared
    words[0] = 5; words[1] = 6; words[2] = 0;
    result = Clear__14VIQueryTriList(words);
    if (result != 0 || calls != 1) return 5;
    if (words[0] != 0 || words[1] != 0 || words[2] != 0) return 6;
    return 0;
}
