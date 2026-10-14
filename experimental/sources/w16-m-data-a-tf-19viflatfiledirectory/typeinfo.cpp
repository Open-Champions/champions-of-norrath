// Observed __tf19VIFlatFileDirectory, 116 bytes at 0x011f3ae8 in the reference image. It returns the node __ti19VIFlatFileDirectory (12 bytes).
// A nonzero first word returns the node at once. When it is zero, the base node __ti10VIFlatFile is registered first with
// __rtti_user under "10VIFlatFile" if its first word is zero; then the node is registered with __rtti_si as "19VIFlatFileDirectory"
// against __ti10VIFlatFile.
// Inferred: two-level chain (10VIFlatFile -> 19VIFlatFileDirectory), with the root registered inline rather than through a getter call.
// Unknown: the node fields after the first word, and the name string contents beyond the class name.
extern "C" {
extern unsigned char __ti19VIFlatFileDirectory[12] __attribute__((aligned(4)));
extern unsigned char __ti10VIFlatFile[8] __attribute__((aligned(4)));
void __rtti_user(void* typeinfo, const char* name);
void __rtti_si(void* typeinfo, const char* name, const void* base);
}

typedef unsigned __attribute__((may_alias)) AliasedWord;
static bool unset(const unsigned char* node) { return *reinterpret_cast<const AliasedWord*>(node) == 0u; }

extern "C" void* __tf19VIFlatFileDirectory() {
    if (unset(__ti19VIFlatFileDirectory)) {
        if (unset(__ti10VIFlatFile)) __rtti_user(__ti10VIFlatFile, "10VIFlatFile");
        __rtti_si(__ti19VIFlatFileDirectory, "19VIFlatFileDirectory", __ti10VIFlatFile);
    }
    return __ti19VIFlatFileDirectory;
}
