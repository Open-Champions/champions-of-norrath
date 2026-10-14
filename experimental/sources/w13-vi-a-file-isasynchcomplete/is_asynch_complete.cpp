// Observed: IsAsynchComplete__6VIFile at 0x010cb250 is 64 bytes. Reads the signed word at
// +0 of the file object. If it is non-negative, calls CDSync__6VIFilei(file, 1) and returns
// 1 when that returns 0, else 0. If it is negative, reads the word at +48 and returns -1
// when it is zero, else 1. No stores.
// Inferred: +0 is the handle/mode word (negative means not open, see Close__6VIFile);
// CDSync with argument 1 is a non-blocking query of a pending disc operation, so the
// return is "complete". The negative-handle path is the in-memory / non-disc source,
// where +48 selects between "nothing pending" (-1) and "complete" (1).
// Unknown: the meaning of the CDSync argument and what +48 holds.
extern "C" {
int CDSync__6VIFilei(void* file, int mode);

int IsAsynchComplete__6VIFile(unsigned char* file) {
    if (*reinterpret_cast<int*>(file) >= 0) {
        return CDSync__6VIFilei(file, 1) == 0;
    }
    return *reinterpret_cast<int*>(file + 48) == 0 ? -1 : 1;
}
}
