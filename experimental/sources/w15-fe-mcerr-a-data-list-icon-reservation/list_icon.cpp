// Observed: sListIcon at 0x01192ca0, 118232 bytes (0x1cdd8; local symbol). The VIMemCard functions (LazyInit,
// Pad, Unpad, PadAsync, Status, ReadFile, WriteFile, GetDir, DeleteFile and their async variants) address it
// together with sMemCardInitialized, sProductTitle and sIconDefinition. In the pinned image about 105,000 of its
// bytes are nonzero, so it carries initialized data, not just storage.
// Inferred: the memory-card list icon (an icon file image the card layer writes next to saved files); this is
// an inference from the name, the size and the users.
// Unknown: its internal layout. The initial bytes are an embedded asset and are NOT reproduced here.
// This unit only reserves the storage with the observed size and 16-byte alignment; it is zero filled, so it is
// a layout reservation and not a reconstruction of the icon. Anything that needs the real icon must supply it
// from a private copy at link or load time.
extern "C" {
unsigned char sListIcon[118232] __attribute__((aligned(16)));
}
