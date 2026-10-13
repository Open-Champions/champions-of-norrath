// Observed: lastkey (0x004865c8) and lasttime (0x004865cc) are two adjacent 4-byte words,
// both zero in the file. The keyboard key reader treats lastkey as the key to repeat and
// adds one to lasttime per frame while it is nonzero.
// Inferred: none beyond the reader's use.
// Unknown: whether any other routine writes them.
extern "C" {
int lastkey = 0;
int lasttime = 0;
}
static_assert(sizeof(lastkey) == 4 && sizeof(lasttime) == 4, "observed sizes");
