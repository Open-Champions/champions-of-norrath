// Observed clone__13LooseIceChunk at 0x00122ac0, 996 bytes.
// Allocates 584 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source): the GameObject table
// pointer (_vt$10GameObject) goes to n+72, the ListHead copy constructor runs for the heads at +0,
// +4 and +8 (it zeroes the destination word and does not read the source), source bytes 12..41,
// 44..57 and 60..71 are copied, the Prop table pointer (_vt$4Prop) replaces n+72, bytes 76..227
// are copied, then bytes 232..319, the PhProp table pointer (_vt$6PhProp) replaces n+72 again,
// bytes 320..321, 328..570 and 572..579 are copied, and the LooseIceChunk table pointer
// (_vt$13LooseIceChunk) is stored last. Bytes 42..43, 58..59, 228..231, 322..327, 571 and 580..583
// are left as the allocator returned them. The new object is returned.
// Copy ranges were measured by running the original on a pattern-filled source (the original
// copies some stretches as 32-byte blocks and one with an alignment test; the result is the same
// set of bytes). Inferred: LooseIceChunk derives from PhProp, which derives from Prop and
// GameObject (the table pointers stored in turn). Unknown: the meaning of the fields and why the
// ranges have the gaps above. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_Prop[] __asm__("_vt$4Prop");
extern unsigned char vt_PhProp[] __asm__("_vt$6PhProp");
extern unsigned char vt_LooseIceChunk[] __asm__("_vt$13LooseIceChunk");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__13LooseIceChunk(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(584));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_Prop);
    copy(n, s, 76, 227);
    copy(n, s, 232, 319);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_PhProp);
    copy(n, s, 320, 321);
    copy(n, s, 328, 570);
    copy(n, s, 572, 579);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_LooseIceChunk);
    return n;
}
