// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "find.cpp"
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static int end_calls; static const void* end_arg;
extern "C" int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* m) { ++end_calls; end_arg = m; return 99; }
static unsigned* node(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 28 * i); }
int main() {
    bool ok = true;
    std::memset(arena, 0x5a, sizeof arena);
    unsigned* m = reinterpret_cast<unsigned*>(arena + 0x100);
    m[2] = 0x400; m[3] = 0;
    unsigned key = 5;
    end_calls = 0;
    if (Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(m, &key) != 99 || end_calls != 1 || end_arg != m) ok = false;
    // tree: root 1 (key 50) -> left 2 (key 20), right 3 (key 0x80000000); 2 -> right 4 (key 30)
    m[3] = 1;
    node(1)[4] = 50; node(1)[2] = 2; node(1)[3] = 3;
    node(2)[4] = 20; node(2)[2] = 0; node(2)[3] = 4;
    node(3)[4] = 0x80000000u; node(3)[2] = 0; node(3)[3] = 0;
    node(4)[4] = 30; node(4)[2] = 0; node(4)[3] = 0;
    struct { unsigned key; int want; } cases[] = {{50, 1}, {20, 2}, {0x80000000u, 3}, {30, 4}, {10, 99}, {40, 99}, {60, 99}, {0xffffffffu, 99}, {0, 99}};
    for (auto& c : cases) {
        key = c.key; end_calls = 0;
        int r = Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(m, &key);
        if (r != c.want || end_calls != (c.want == 99)) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
