// Observed: pmlistptrs is a global object of 1024 bytes at 0x00435b68, zero in the file;
// pmlistptrsused is a 4-byte global at 0x00485e1c, zero. The spawn routine indexes the
// object as pointers.
// Inferred: 256 four-byte slots on the target (1024 / 4), which is why the array has 256
// elements here; the importer declares it as an array of pointers without a size.
// Unknown: the maximum the spawn routine allows, and the producers of the list.
extern "C" {
char* pmlistptrs[256] = {};
int pmlistptrsused = 0;
}
#if defined(__mips__)
static_assert(sizeof(pmlistptrs) == 1024, "observed size");
#endif
