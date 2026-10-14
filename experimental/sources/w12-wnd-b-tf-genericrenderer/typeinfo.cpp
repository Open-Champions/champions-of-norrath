// Observed __tf20VIWndGenericRenderer at 0x012094e8, 80 bytes (weak).
// Returns the 12-byte type_info node __ti20VIWndGenericRenderer (0x01227d40; zero in the image, defined here because
// no other unit owns it). Only the first word of the node gates initialization. When it is zero:
// call __tf5VIWnd, then __rtti_si(node, "20VIWndGenericRenderer", __ti5VIWnd). A nonzero first word skips both
// calls. The node is returned either way. The text pointer is the string at 0x011d4ba8 in the
// image, which reads "20VIWndGenericRenderer".
// Inferred: the node is the class's runtime type descriptor and __rtti_si records a single-base
// class, from the symbol names.
// Unknown: the rest of the node's layout (only the first word is tested here).
extern "C" {
unsigned char __ti20VIWndGenericRenderer[12] __attribute__((aligned(4)));
extern unsigned char __ti5VIWnd[] __attribute__((aligned(4)));
void* __tf5VIWnd();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf20VIWndGenericRenderer() {
    if (unset(__ti20VIWndGenericRenderer)) {
        __tf5VIWnd();
        __rtti_si(__ti20VIWndGenericRenderer, "20VIWndGenericRenderer", __ti5VIWnd);
    }
    return __ti20VIWndGenericRenderer;
}
