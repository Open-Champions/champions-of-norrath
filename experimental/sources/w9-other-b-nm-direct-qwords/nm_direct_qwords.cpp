// Observed: nmDirectQWords is one 32-bit word at 0x0048699c in zero-filled storage. The neighbours are
// nmChars before it and restrictChars 4 bytes after. _fontPrintu__FiiPUsi, fontColor__FUiii and
// fontScissor__Fiiii load and store it as a word; fontEnd__Fv loads its low halfword unsigned.
// Inferred: a count of quadwords written directly into the font packet, from the name and the way the
// font routines use it.
// Unknown: the unit of the count and whether the halfword read in fontEnd__Fv is a deliberate truncation.

extern "C" {
int nmDirectQWords;
}
static_assert(sizeof(nmDirectQWords) == 4, "nmDirectQWords");
