// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "find.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace; static int find_result, end_result; static unsigned seen_key; static const void* seen_map;
extern "C" int Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi(const void* m, const unsigned* k) { trace += "find;"; seen_map = m; seen_key = *k; return find_result; }
extern "C" int End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry(const void* m) { trace += m == seen_map ? "end;" : "end?;"; return end_result; }
int main() {
    bool ok = true;
    std::memset(arena, 0x5a, sizeof arena);
    unsigned* m = reinterpret_cast<unsigned*>(arena + 0x100);
    m[2] = 0x400;
    unsigned* entry = reinterpret_cast<unsigned*>(arena + 0x400 + 28 * 3 + 20);
    entry[0] = 0xaa; entry[1] = 0xfffffff0u;
    unsigned type = 1; int value = 2;
    find_result = 7; end_result = 7; trace.clear();
    if (Find__12VIDictionaryUiR14VIResourceTypeRi(m, 1234, &type, &value) != 0 || trace != "find;end;" || type != 1 || value != 2 || seen_key != 1234) ok = false;
    find_result = 3; end_result = 8; trace.clear();
    if (Find__12VIDictionaryUiR14VIResourceTypeRi(m, 77, &type, &value) != 1 || trace != "find;end;" || type != 0xaa || value != -16 || seen_key != 77 || seen_map != m) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
