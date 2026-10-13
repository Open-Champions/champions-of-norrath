// Observed __ps2_klibinfo__ at 0x00444940, 16 bytes of initialized data (the next object starts
// at 0x00444950). The bytes are the text "PsIIlibkernl2800" with no terminator. The two
// version-check routines _lf_version and _fs_version read the last four bytes ("2800") of it.
// Their sources keep a private copy (a const array has internal linkage in C++, so those copies
// are not visible to the linker); this unit gives the object its external symbol.
// Inferred only: that the text names the kernel library and its version, from its contents.
// Unknown: whether more fields were meant to follow; the image has none inside these 16 bytes.
extern "C" {
extern const unsigned char __ps2_klibinfo__[16];
const unsigned char __ps2_klibinfo__[16] = {'P', 's', 'I', 'I', 'l', 'i', 'b', 'k', 'e', 'r', 'n', 'l', '2', '8', '0', '0'};
}
