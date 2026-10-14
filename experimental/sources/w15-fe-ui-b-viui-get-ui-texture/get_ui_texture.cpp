// Observed: GetUITexture__F9VIUITexId at 0x01035e88, 56 bytes. The argument is compared as an unsigned
// number with 251. Below that, the result is UITextures + id * 36; otherwise the result is UITextures itself
// (entry 0). UITextures is a file-local symbol at 0x011c6f70 whose ELF size is 0x234c = 251 * 36 bytes.
// Inferred: UITextures is a table of 251 records of 36 bytes, and an out-of-range id falls back to record 0.
// Callers elsewhere read words 4 and 5 of a record as a size; the record layout is not defined here.
// Unknown: whether the table is initialised data or filled at run time (it is not defined here: left as an
// unresolved import), and the names of the ids.
extern "C" {
extern unsigned char UITextures[];

unsigned char* GetUITexture__F9VIUITexId(unsigned id) {
    if (id < 251u) return UITextures + id * 36u;
    return UITextures;
}
}
