// Observed: AbortOpen__9VIObjFile at 0x010a6468, 60 bytes. Offsets are bytes from the object file. Stores the 64-bit value -1
// at +224, zero at +560, zero at +200, +204 and +208, and the 64-bit value zero at +216, then calls Close__6VIFile(this) (the base
// file close, on the same address) and returns 0.
// Inferred: the object-file header fields written by PostOpen/WriteFileHeader (magic at +200, data start at +216, write cursor
// at +224) are reset and the "open" word at +560 cleared, so a failed open leaves no state behind.
// Unknown: names of the words, and Close's result (ignored).
extern "C" {
int Close__6VIFile(void* file);

int AbortOpen__9VIObjFile(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<long long*>(bytes + 224) = -1;
    *reinterpret_cast<unsigned*>(bytes + 560) = 0;
    *reinterpret_cast<unsigned*>(bytes + 200) = 0;
    *reinterpret_cast<unsigned*>(bytes + 204) = 0;
    *reinterpret_cast<unsigned*>(bytes + 208) = 0;
    *reinterpret_cast<long long*>(bytes + 216) = 0;
    Close__6VIFile(object);
    return 0;
}
}
