// Observed: iconHeader is the 964-byte block at the address MC_Init clears
// and the later card command sends with length 964. The image bytes are
// zero. The next symbol starts four bytes after that length.
// Inferred: none.
// Unknown: which of the 964 bytes stay zero after the header stores.
extern "C" unsigned char iconHeader[964] = {};
static_assert(sizeof(iconHeader) == 964, "icon header");
