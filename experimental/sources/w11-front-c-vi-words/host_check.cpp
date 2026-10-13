// Host check for the VI words. Not an EE plan input.
extern "C" {
extern void* sMessagePool;
extern unsigned char sWndMcErr[0x210];
}
int main() {
    static_assert(sizeof(sWndMcErr) == 0x210, "observed size");
    if (sMessagePool) return 1;
    for (unsigned char c : sWndMcErr) if (c) return 2;
    return 0;
}
