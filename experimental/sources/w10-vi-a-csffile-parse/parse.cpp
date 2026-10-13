// Observed: Parse__9VICSFFilePCcR10VIESFParseR7VIScene at 0x010a4bf0 is 192
// bytes. Arguments: this, path, VIESFParse reference, VIScene reference. It
// builds a VIFile in a local buffer (__6VIFile), prints "Decompressing %s\n"
// with the path, and calls Open__6VIFilePCci(file, path, 33). If the open
// result is negative it destroys the file (_$_6VIFile with 2) and returns -1.
// Otherwise it calls the overload Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene
// (this, file, parse, scene), destroys the file with 2, and returns -1 if that
// parse result was negative, else 0.
// Unknown: the size of VIFile; the local buffer is 192 bytes, which is the most
// the original frame could hold below its saved registers.
extern "C" {
void __6VIFile(void* file);
void _$_6VIFile(void* file, int flags);
int Open__6VIFilePCci(void* file, const char* path, int mode);
int Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(void* object, void* file, void* parse, void* scene);
int printf(const char* format, ...);

int Parse__9VICSFFilePCcR10VIESFParseR7VIScene(void* object, const char* path, void* parse, void* scene) {
    alignas(16) unsigned char file[192];
    __6VIFile(file);
    printf("Decompressing %s\n", path);
    if (Open__6VIFilePCci(file, path, 33) < 0) {
        _$_6VIFile(file, 2);
        return -1;
    }
    int result = Parse__9VICSFFileR6VIFileR10VIESFParseR7VIScene(object, file, parse, scene);
    _$_6VIFile(file, 2);
    return result < 0 ? -1 : 0;
}
}
