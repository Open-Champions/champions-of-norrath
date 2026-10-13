// Observed: Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode at 0x010a62e0 is
// 196 bytes. Arguments: the object file (a0), an int (a1), a MemSegm pointer (a2), a long
// (a3) and a FileMode value (t0, named mode). If the word at +560 is nonzero it first calls
// Close__9VIObjFile(this). It then chooses a flag value from the mode: 33 when mode is 0,
// 50 when mode is 1, 42 for any other value, and calls
//   Open__6VIFileiPQ26VIFile7MemSegmil(this, a1, a2, flag, a3)
// (the base-class open; the long goes in the fifth register). A negative result returns -1
// with nothing else changed. Otherwise it stores mode at +192 and 1 at +560 and tail-calls
// PostOpen__9VIObjFile(this), whose result is the return value.
// Inferred: +560 is an "open" flag and +192 stores the mode; 33, 50 and 42 are VIFile open
// flags for the three modes. The mapping is observed, but which mode is read, write or
// append is not established here. Unknown: member names and what the long argument means.
extern "C" {
void Close__9VIObjFile(void* file);
int Open__6VIFileiPQ26VIFile7MemSegmil(void* file, int a, void* segment, int flags, long size);
int PostOpen__9VIObjFile(void* file);

int Open__9VIObjFileiPQ26VIFile7MemSegmlQ29VIObjFile8FileMode(unsigned char* self, int a, void* segment, long size, int mode) {
    if (*reinterpret_cast<int*>(self + 560) != 0) Close__9VIObjFile(self);
    int flags = mode == 0 ? 33 : (mode == 1 ? 50 : 42);
    if (Open__6VIFileiPQ26VIFile7MemSegmil(self, a, segment, flags, size) < 0) return -1;
    *reinterpret_cast<int*>(self + 192) = mode;
    *reinterpret_cast<int*>(self + 560) = 1;
    return PostOpen__9VIObjFile(self);
}
}
