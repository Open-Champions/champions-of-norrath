// Observed: FileType__9VIObjFile at 0x010a65b0, 8 bytes. With the object file record as
// this, it returns the 32-bit word at this + 208 and touches nothing else.
// Inferred: the file-type word; the name suffix C shows the method is const.
// Unknown: the member name and the full set of values the word can hold.
extern "C" {
int FileType__9VIObjFile(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(object) + 208);
}
}
