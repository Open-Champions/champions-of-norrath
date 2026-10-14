// Observed clipFrustumNormalSign at 0x006ec7a8 (.bss). The authored world frustum code
// writes one signed byte per axis for each of six clip planes (indices 3 * plane + axis,
// 18 bytes), and the clip-cube code reads the same 18 bytes. The sibling frustumNormalSign
// at 0x006ec790 is followed by the next symbol after 24 bytes, which is consistent with
// 18 bytes plus alignment; after this symbol the next named symbol is 88 bytes away, so
// the size 18 is inferred from the uses, not observed. Meaning inferred from the name.
extern "C" {
signed char clipFrustumNormalSign[18];
}
