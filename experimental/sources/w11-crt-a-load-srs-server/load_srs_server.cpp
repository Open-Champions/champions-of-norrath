// Observed LoadSRSServer__Fv at 0x010b9000, 268 bytes, pinned SLUS_205.65. No arguments; returns
// a C string pointer or null. It works with a local VIFile (192 bytes of stack, as in the other
// VIFile users) and a 512-byte file-local buffer, filebuf.3356 at 0x011e8150, which stays
// valid after the return because the result points into it.
//   1. Open the file named by the string at 0x011d4548 ("data/ui/srs.txt") with mode 33.
//      A nonzero Open result goes to the failure exit below.
//   2. Seek(0, 2) and Tell: the size, taken as a signed 32-bit value; zero or less is also a
//      failure.
//   3. Clear the 512-byte buffer, Seek(0, 1), Read 512 bytes into it (the read count is not
//      looked at), Close the file.
//   4. Cut the text at the first line feed and then at the first carriage return, whichever
//      is found (each strchr runs on the buffer start, so the second one sees the first cut).
//   5. printf("Loaded SRS server = %s\n", buffer), destroy the file with flag 2 and return
//      the buffer.
//   Failure exit: destroy the file with flag 2 and return null (the open file is not closed
//   on the zero-size path, which only the destructor handles).
// Inferred: the buffer holds the name of the server read from the file's first line; the
// origin values 2 and 1 are numbers of the VIFile::SeekOrigin enum, whose members are not known
// here, so the raw numbers are passed. Unknown: what Open's mode 33 is made of.
// Nothing was executed on the EE.
extern "C" {
void __6VIFile(void* file);
void _$_6VIFile(void* file, int flags);
int Open__6VIFilePCci(void* file, const char* path, int mode);
int Seek__6VIFilelQ26VIFile10SeekOrigin(void* file, long offset, int origin);
long Tell__6VIFile(void* file);
int Read__6VIFilePvi(void* file, void* destination, int count);
void Close__6VIFile(void* file);
void* memset(void* destination, int value, __SIZE_TYPE__ count);
char* strchr(const char* text, int character);
int printf(const char* format, ...);

char* LoadSRSServer__Fv() {
    static char filebuf[512];
    alignas(16) unsigned char file[192];
    __6VIFile(file);
    if (Open__6VIFilePCci(file, "data/ui/srs.txt", 33) == 0) {
        Seek__6VIFilelQ26VIFile10SeekOrigin(file, 0, 2);
        const int size = static_cast<int>(Tell__6VIFile(file));
        if (size > 0) {
            memset(filebuf, 0, 512);
            Seek__6VIFilelQ26VIFile10SeekOrigin(file, 0, 1);
            Read__6VIFilePvi(file, filebuf, 512);
            Close__6VIFile(file);
            char* cut = strchr(filebuf, 10);
            if (cut) *cut = 0;
            cut = strchr(filebuf, 13);
            if (cut) *cut = 0;
            printf("Loaded SRS server = %s\n", filebuf);
            _$_6VIFile(file, 2);
            return filebuf;
        }
    }
    _$_6VIFile(file, 2);
    return 0;
}
}
