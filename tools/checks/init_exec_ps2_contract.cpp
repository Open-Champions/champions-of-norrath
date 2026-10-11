#include <cstdio>
static int needed = 1, setups, copies, entries, flushes[4];
static unsigned setup_a[8], setup_b[8], copy_dest, copy_len, ids[4];
extern "C" unsigned SysExecPS2Entry[8] = {1, 2, 3, 4, 10, 0, 11, 0};
extern "C" unsigned char osdsrc[1960] = {};
extern "C" int PatchIsNeeded() { return needed; }
extern "C" void kernel_execps2_setup(unsigned a, unsigned b) {
    setup_a[setups] = a;
    setup_b[setups] = b;
    ++setups;
}
extern "C" void kernel_execps2_copy(unsigned dest, const void* source, unsigned bytes) {
    if (source != osdsrc) __builtin_trap();
    copy_dest = dest;
    copy_len = bytes;
    ++copies;
}
extern "C" unsigned kernel_execps2_get_entry(unsigned id) { ids[entries] = id; return 100 + entries++; }
extern "C" int FlushCache(int mode) { ++flushes[mode & 3]; return 0; }
extern "C" void InitExecPS2();
int main() {
    needed = 0;
    InitExecPS2();
    if (setups || copies || entries || flushes[0] || flushes[2]) return 1;
    needed = 1;
    InitExecPS2();
    if (copies != 1 || copy_dest != 0x80074000u || copy_len != 1960) return 2;
    if (flushes[0] != 1 || flushes[2] != 1 || entries != 1 || ids[0] != 10) return 3;
    if (setups != 3 || setup_a[0] != 1 || setup_b[0] != 2 || setup_a[1] != 3 || setup_b[1] != 4) return 4;
    if (setup_a[2] != 10 || setup_b[2] != 100) return 5;
    std::puts("execps2 patch orchestration passed");
}
