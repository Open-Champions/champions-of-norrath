// Observed: Flush__9VIObjFile at 0x010a6550, 88 bytes. Offsets are bytes from the object file. If the word at +560 is zero it returns -1.
// Otherwise, if the word at +572 is nonzero it returns -1 too. Otherwise, when the word at +588 is nonzero it calls
// WriteFileHeader__9VIObjFilei(this, 0); then it calls Flush__6VIFile(this) and returns 0.
// Inferred: +560 is the open flag and +572 a state in which flushing is refused (Close writes the stream end first); +588 marks
// header data changed, so it is written back before the file flush.
// Unknown: member names and the results of the two callees (ignored).
extern "C" {
int WriteFileHeader__9VIObjFilei(void* object, int flag);
int Flush__6VIFile(void* file);

int Flush__9VIObjFile(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (*reinterpret_cast<unsigned*>(bytes + 560) == 0) return -1;
    if (*reinterpret_cast<unsigned*>(bytes + 572) != 0) return -1;
    if (*reinterpret_cast<unsigned*>(bytes + 588) != 0) WriteFileHeader__9VIObjFilei(object, 0);
    Flush__6VIFile(object);
    return 0;
}
}
