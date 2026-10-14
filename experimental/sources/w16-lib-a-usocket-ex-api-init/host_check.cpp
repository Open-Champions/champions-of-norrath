// Host check, not a plan input.
#include <cstdio>
#include <cstdlib>
#include "init.cpp"
void* sMessagePool; unsigned char sFreeMessages[16]; unsigned char sUsedMessages[16];
static int inits[2], init_n, adds; static unsigned char* nodes[16]; static unsigned malloc_size; static unsigned char block[0x8240];
extern "C" void* __wrap_malloc(unsigned n) { malloc_size = n; return block; }
extern "C" void ll_init__FP5llist(llist* l) { if (init_n < 2) inits[init_n] = reinterpret_cast<unsigned char*>(l) == sFreeMessages ? 1 : (reinterpret_cast<unsigned char*>(l) == sUsedMessages ? 2 : 0); ++init_n; }
extern "C" void ll_add_tail__FP5llistP5lnode(llist* l, lnode* n) { if (reinterpret_cast<unsigned char*>(l) != sFreeMessages) std::exit(9); if (adds < 16) nodes[adds] = reinterpret_cast<unsigned char*>(n); ++adds; }
int main() {
    VIInitUSocketExApi__Fv();
    if (malloc_size != 0x8240 || sMessagePool != block || init_n != 2 || inits[0] != 1 || inits[1] != 2 || adds != 16) return 1;
    for (int i = 0; i < 16; ++i) if (nodes[i] != block + i * 2084) return 2;
    std::puts("usocket init check passed");
    return 0;
}
