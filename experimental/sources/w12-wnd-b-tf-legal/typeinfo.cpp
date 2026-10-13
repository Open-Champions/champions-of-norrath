// Observed __tf10VIWndLegal at 0x0120aa78, 80 bytes (weak).
// Returns the 12-byte type_info node __ti10VIWndLegal (0x01227d50; zero in the image, defined here because
// no other unit owns it). Only the first word of the node gates initialization. When it is zero:
// call __tf5VIWnd, then __rtti_si(node, "10VIWndLegal", __ti5VIWnd). A nonzero first word skips both
// calls. The node is returned either way. The text pointer is the string at 0x011d4e28 in the
// image, which reads "10VIWndLegal".
// Inferred: the node is the class's runtime type descriptor and __rtti_si records a single-base
// class, from the symbol names.
// Unknown: the rest of the node's layout (only the first word is tested here).
extern "C" {
unsigned char __ti10VIWndLegal[12] __attribute__((aligned(4)));
extern unsigned char __ti5VIWnd[] __attribute__((aligned(4)));
void* __tf5VIWnd();
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf10VIWndLegal() {
    if (unset(__ti10VIWndLegal)) {
        __tf5VIWnd();
        __rtti_si(__ti10VIWndLegal, "10VIWndLegal", __ti5VIWnd);
    }
    return __ti10VIWndLegal;
}
