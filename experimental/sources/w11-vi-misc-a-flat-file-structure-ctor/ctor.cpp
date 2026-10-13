// Observed: __19VIFlatFileStructure at 0x01004fe8 is 132 bytes and returns nothing.
// On the object: word 12 = 2; word 8 = -1; word 16 = the address of the data symbol
// _vt$19VIFlatFileDirectory (0x011f3a78, 24 bytes); word 0 = 0; word 4 = 0 (before
// the call); __t6VIList1ZP10VIFlatFile(this + 24) is called; word 20 = 0. Then the
// two-byte string "/" is duplicated: if the old word 0 is nonzero it is passed to
// __wrap_free, and word 0 receives the result of __wrap_strdup("/"). Because word 0
// was just cleared, the free is never reached in practice; the original still
// tests it (the inlined base constructor and a name setter are inferred).
// Unknown: field meanings other than what the symbol names show.
extern "C" {
extern char norrath_flat_file_directory_vt[] __asm__("_vt$19VIFlatFileDirectory");
void __t6VIList1ZP10VIFlatFile(void* list);
void __wrap_free(void* pointer);
char* __wrap_strdup(const char* text);

void __19VIFlatFileStructure(void* object) {
    char* bytes = static_cast<char*>(object);
    char** name = reinterpret_cast<char**>(bytes);
    *reinterpret_cast<unsigned*>(bytes + 12) = 2;
    *reinterpret_cast<int*>(bytes + 8) = -1;
    *reinterpret_cast<char**>(bytes + 16) = norrath_flat_file_directory_vt;
    *name = nullptr;
    *reinterpret_cast<unsigned*>(bytes + 4) = 0;
    __t6VIList1ZP10VIFlatFile(bytes + 24);
    *reinterpret_cast<unsigned*>(bytes + 20) = 0;
    if (*name != nullptr) __wrap_free(*name);
    *name = __wrap_strdup("/");
}
}
