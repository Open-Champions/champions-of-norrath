// Observed: PostOpen__9VIObjFile at 0x010a4d98, 320 bytes. With the object file as this;
// M is the word at +192 (the open mode):
//  - M == 2 (create): stores 0x4f424a46 at +200, the 64-bit value 32 at +216, the 64-bit
//    value -1 at +224, zero at +204, +208 and +212, then calls WriteFileHeader(this, 0).
//  - otherwise: Seek(this, 0, 2); if Tell(this) (a 64-bit result) is below 32 it calls
//    AbortOpen and returns -1. Else ReadFileHeader(this, 0); if the word at +200 is not
//    0x4f424a46 it calls AbortOpen and returns -2. Else if M == 1 it calls Seek(this, 0, 2);
//    otherwise, when Tell(this) differs from the 64-bit value at +216, it calls
//    Seek(this, that value, 1).
// On every path that did not abort: if the word at +192 reads non-zero then +564 = 1 and
// +568 = 0, else +564 = 0 and +568 = 1; +580, +584 and +588 are zeroed; the result is zero.
// Inferred: 0x4f424a46 is the file magic (bytes "FJBO" little-endian); +216 is the data
// start offset and +224 a write cursor; +564/+568 are read/write flags. The seek origin
// values 1 and 2 are passed on as observed, without a name for them.
// Unknown: member names, and why the final seek is relative to the current position.
extern "C" {
int Seek__6VIFilelQ26VIFile10SeekOrigin(void* file, long long offset, int origin);
long long Tell__6VIFile(void* file);
int WriteFileHeader__9VIObjFilei(void* object, int arg);
int ReadFileHeader__9VIObjFilei(void* object, int arg);
int AbortOpen__9VIObjFile(void* object);

int PostOpen__9VIObjFile(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    int* mode = reinterpret_cast<int*>(bytes + 192);
    int* magic = reinterpret_cast<int*>(bytes + 200);
    long long* start = reinterpret_cast<long long*>(bytes + 216);
    long long* cursor = reinterpret_cast<long long*>(bytes + 224);
    if (*mode == 2) {
        *magic = 0x4f424a46;
        *start = 32;
        *cursor = -1;
        *reinterpret_cast<int*>(bytes + 204) = 0;
        *reinterpret_cast<int*>(bytes + 208) = 0;
        *reinterpret_cast<int*>(bytes + 212) = 0;
        WriteFileHeader__9VIObjFilei(object, 0);
    } else {
        Seek__6VIFilelQ26VIFile10SeekOrigin(object, 0, 2);
        if (Tell__6VIFile(object) < 32) {
            AbortOpen__9VIObjFile(object);
            return -1;
        }
        ReadFileHeader__9VIObjFilei(object, 0);
        if (*magic != 0x4f424a46) {
            AbortOpen__9VIObjFile(object);
            return -2;
        }
        if (*mode == 1) {
            Seek__6VIFilelQ26VIFile10SeekOrigin(object, 0, 2);
        } else if (Tell__6VIFile(object) != *start) {
            Seek__6VIFilelQ26VIFile10SeekOrigin(object, *start, 1);
        }
    }
    if (*mode != 0) {
        *reinterpret_cast<int*>(bytes + 564) = 1;
        *reinterpret_cast<int*>(bytes + 568) = 0;
    } else {
        *reinterpret_cast<int*>(bytes + 564) = 0;
        *reinterpret_cast<int*>(bytes + 568) = 1;
    }
    *reinterpret_cast<int*>(bytes + 584) = 0;
    *reinterpret_cast<int*>(bytes + 580) = 0;
    *reinterpret_cast<int*>(bytes + 588) = 0;
    return 0;
}
}
