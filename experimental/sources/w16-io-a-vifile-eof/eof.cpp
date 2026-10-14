// Observed: Eof__6VIFile at 0x010caf78, 48 bytes. Argument: the file. The result is the word at +8, except when the signed word at
// +0 is negative and both the word at +48 and the word at +56 are zero; then it is -1.
// Inferred: +8 is the end-of-data flag that the read and seek routines clear and set; the -1 case is a file with no backing store,
// the same combination that makes Read__6VIFilePvi return -1.
// Unknown: the member names.
extern "C" {
int Eof__6VIFile(unsigned char* file) {
    if (*reinterpret_cast<int*>(file) < 0 && *reinterpret_cast<int*>(file + 48) == 0 && *reinterpret_cast<int*>(file + 56) == 0) return -1;
    return *reinterpret_cast<int*>(file + 8);
}
}
