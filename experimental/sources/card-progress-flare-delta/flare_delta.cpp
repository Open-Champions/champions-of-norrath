// Observed: this flareDelta is the 4-byte card word immediately after the
// card flareAlpha. MC_Init stores zero with a word store. The image bytes
// are zero. Another local flareDelta is a different object.
// Inferred: none.
// Unknown: which routine stores a nonzero delta.
extern "C" int flareDelta = 0;
